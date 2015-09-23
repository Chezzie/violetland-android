#include <iostream>
#include <exception>

// #include <libintl.h>
#include <Translation.h>
#include <locale.h>
#include <boost/static_assert.hpp>

#include "InputHandler.h"
#include "game/HUD.h"
#include "game/GameState.h"
#include "system/graphic/Camera.h"
#include "system/graphic/VideoManager.h"
#include <JNIBridge.h>

#define _(STRING)            gettext(STRING)
#define MAX_CHARACTERS 20
using namespace std;

extern Camera* cam;
// extern VideoManager* videoManager;
extern violetland::HUD* hud;
extern violetland::GameState* gameState;
extern uint32_t g_timeInPause;

bool g_isInPoll;
bool g_isExit;

string InputHandler::m_eventIdentifiers[] = {
	"Restart", 
	"Exit", 
	"Menu", 
	"MenuClickA", 
	"MenuClickB", 
	"Toggle Light", 
	"Toggle Laser", 
	"Show Char", 
	"Pause", 
	"Move Left", 
	"Move Right", 
	"Move Up", 
	"Move Down", 
	"Help", 
	"Pick Up", 
	"Throw Grenade", 
	"Fire", 
	"Reload", 
	"Teleport", 
};

string InputHandler::m_eventNames[GameInputEventsCount];

void InputHandler::initEventNames() {
	string names[] = {
		_("Restart"), 
		_("Exit"), 
		_("Menu"), 
		_("MenuClickA"), 
		_("MenuClickB"), 
		_("Toggle Light"), 
		_("Toggle Laser"), 
		_("Show Char"), 
		_("Pause"), 
		_("Move Left"), 
		_("Move Right"), 
		_("Move Up"), 
		_("Move Down"), 
		_("Help"), 
		_("Pick Up"), 
		_("Throw Grenade"), 
		_("Fire"), 
		_("Reload"), 
		_("Teleport"), 
	};
	BOOST_STATIC_ASSERT(sizeof(names) / sizeof(string) == GameInputEventsCount);
	
	for (unsigned i = 0; i < GameInputEventsCount; ++i)
		m_eventNames[i] = names[i];
}

InputHandler::InputHandler(Binding* binding) {
	BOOST_STATIC_ASSERT(sizeof(m_eventNames) / sizeof(string) == GameInputEventsCount);
	
	std::cout << "InputHandler..." << std::endl;

	for (int i = 0; i < GameInputEventsCount; i++) {
		m_event[i] = false;
	}

	m_binding = binding;

#ifdef __ANDROID__
	memset(fingerXY, 0, sizeof(fingerXY));
	target.y = -cam->getHalfH() / 3;
	target.x = 0;
	isFiring = false;
	fingerIdWasd = -1;
	fingerIdShoot = -1;
	fingerIdChar = -1;
	fingerIdReload = -1;
	fingerIdGrenade = -1;
	ptTeleport.x = -1;
	ptTeleport.y = -1;
#else
	mouseX = mouseY = 0;
#endif

	m_mode = Direct;
	m_textValidated = false;
	m_textContent = "";
	m_curTextPos = 0;
}

void InputHandler::setInputMode(InputMode mode) {
	/* if (m_mode == mode)
		return;

	if (mode == Text || mode == TextMandatory) {
		m_textValidated = false;
		m_textContent = "";
		m_curTextPos = 0;
	} */

	m_mode = mode;
}

/* void InputHandler::setInputModeText(bool mandatory, std::string text) {
	if ((!mandatory && m_mode == Text)
			|| (mandatory && m_mode == TextMandatory))
		return;

	if (mandatory)
		setInputMode(TextMandatory);
	else
		setInputMode(Text);

	m_textContent = text;
	m_curTextPos = strlen(m_textContent.c_str());
} */

bool InputHandler::getPressInput(GameInputEvents evnt) {
	if (m_event[evnt]) {
		m_event[evnt] = false;
		return true;
	} else {
		return false;
	}
}

void InputHandler::resetMouseButtons() {
	processEvent(Mouse, false, SDL_BUTTON_LEFT);
	processEvent(Mouse, false, SDL_BUTTON_RIGHT);
	processEvent(Mouse, false, SDL_BUTTON_MIDDLE);
#ifdef __ANDROID__
	memset(fingerXY, 0, sizeof(fingerXY));
	isFiring = false;
	fingerIdWasd = -1;
	fingerIdShoot = -1;
	fingerIdChar = -1;
	fingerIdReload = -1;
	fingerIdGrenade = -1;
	ptTeleport.x = -1;
	ptTeleport.y = -1;
#endif
}

void InputHandler::processEvent(BindingType type, bool down, int value) {
	for (int i = 0; i < GameInputEventsCount; i++) {
		if (m_binding[i].Type == type && m_binding[i].Value == value)
		{
#ifdef __ANDROID__
			if (value == SDLK_AC_BACK && !down)
				continue;
#endif
			m_event[i] = down;
		}
	}
}

/* void InputHandler::processTextInput(SDL_Event sdlEvent) {
	switch (sdlEvent.key.keysym.sym) {
	case SDLK_ESCAPE:
		setInputMode(Direct);
		break;
	case SDLK_BACKSPACE:
		if (m_curTextPos > 0)
			m_textContent.erase(--m_curTextPos, 1);
		break;
	case SDLK_RETURN:
		if ((m_mode == TextMandatory && m_textContent.size() >= 1) || m_mode
				== Text) {
			m_textValidated = true;
		}
		break;
	default:
#if 0
		if (sdlEvent.key.keysym.unicode < 127 && m_curTextPos < MAX_CHARACTERS) {
			char c = sdlEvent.key.keysym.unicode;
			if (' ' <= c && c <= '~')
				m_textContent.insert(m_curTextPos++, 1, c);
		}
#endif
		break;
	}
} */

void InputHandler::process() {
	SDL_Event sdlEvent;

	while (true) {
		g_isInPoll = true;
		Uint32 startTick = SDL_GetTicks();
		int pollResult = SDL_PollEvent(&sdlEvent);
		g_isInPoll = false;
		Uint32 elapsed = SDL_GetTicks() - startTick;
		if (elapsed > 100) {
			LOGFMT("PollEvent took: %u", elapsed);
		}
        g_timeInPause += elapsed;
#ifdef __ANDROID__
		if (elapsed > 750) {
			LOG("Switching to pause.");
			if (!gameState->Paused && !gameState->Charstats && !gameState->Lost) {
				processEvent(Keyboard, true, SDLK_AC_BACK);
			}
		}
#endif
		if (!pollResult) {
			break;
		}
		switch (sdlEvent.type) {
		case SDL_KEYDOWN:
			if (m_mode == Direct)
				processEvent(Keyboard, true, sdlEvent.key.keysym.sym);
			/*if (m_mode == Text || m_mode == TextMandatory)
				processTextInput(sdlEvent);*/
			break;
		case SDL_KEYUP:
			processEvent(Keyboard, false, sdlEvent.key.keysym.sym);
			break;
#ifndef __ANDROID__
		case SDL_MOUSEBUTTONDOWN:
			processEvent(Mouse, true, sdlEvent.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			processEvent(Mouse, false, sdlEvent.button.button);
			break;
		case SDL_MOUSEMOTION:
			mouseX = sdlEvent.motion.x;
			mouseY = sdlEvent.motion.y;
			break;
#else
		case SDL_FINGERUP:
			if (sdlEvent.tfinger.fingerId >= 5)
				break;
			if (!(gameState->Begun && !gameState->Paused && !gameState->Charstats)) {
				processEvent(Mouse, false, SDL_BUTTON_LEFT);
			} else if (sdlEvent.tfinger.fingerId == fingerIdChar) {
				// processEvent(Keyboard, false, SDLK_c);
				fingerIdChar = -1;
			} else if (sdlEvent.tfinger.fingerId == fingerIdGrenade) {
				// processEvent(Keyboard, false, SDLK_g);
				fingerIdGrenade = -1;
			} else if (sdlEvent.tfinger.fingerId == fingerIdReload) {
				// processEvent(Keyboard, false, SDLK_r);
				fingerIdReload = -1;
			} else if (sdlEvent.tfinger.fingerId == fingerIdShoot) {
				fingerXY[sdlEvent.tfinger.fingerId].x = 0;
				fingerXY[sdlEvent.tfinger.fingerId].y = 0;
				isFiring = false;
				fingerIdShoot = -1;
			} else if (sdlEvent.tfinger.fingerId == fingerIdWasd) {
				fingerXY[sdlEvent.tfinger.fingerId].x = 0;
				fingerXY[sdlEvent.tfinger.fingerId].y = 0;
				fingerIdWasd = -1;
			}
			break;
		case SDL_FINGERDOWN:
			if (sdlEvent.tfinger.fingerId >= 5) {
				break;
			} else {
				SDL_Rect viewport;
				glGetIntegerv(GL_VIEWPORT, (GLint*)&viewport);
				fingerXY[sdlEvent.tfinger.fingerId].x = int(sdlEvent.tfinger.x * viewport.w);
				fingerXY[sdlEvent.tfinger.fingerId].y = int(sdlEvent.tfinger.y * viewport.h);
				if (gameState->Begun && !gameState->Paused && !gameState->Charstats) {
					if (hud->isPointInRect(violetland::HUD::AREA_CHAR, fingerXY[sdlEvent.tfinger.fingerId], NULL)) {
						processEvent(Keyboard, true, SDLK_c);
						fingerIdChar = sdlEvent.tfinger.fingerId;
					} else if (hud->isPointInRect(violetland::HUD::AREA_GRENADE, fingerXY[sdlEvent.tfinger.fingerId], NULL)) {
						processEvent(Keyboard, true, SDLK_g);
						fingerIdGrenade = sdlEvent.tfinger.fingerId;
					} else if (hud->isPointInRect(violetland::HUD::AREA_RELOAD, fingerXY[sdlEvent.tfinger.fingerId], NULL)) {
						processEvent(Keyboard, true, SDLK_r);
						fingerIdReload = sdlEvent.tfinger.fingerId;
					} else if (hud->isPointInRect(violetland::HUD::AREA_SHOOT, fingerXY[sdlEvent.tfinger.fingerId], NULL)) {
						fingerIdShoot = sdlEvent.tfinger.fingerId;
						isFiring = true;
						goto motion;
					} else if (hud->isPointInRect(violetland::HUD::AREA_WASD, fingerXY[sdlEvent.tfinger.fingerId], NULL)) {
						fingerIdWasd = sdlEvent.tfinger.fingerId;
					} else {
						static Uint32 lastTimeStamp = 0;
						Uint32 curTimeStamp = SDL_GetTicks();
						if (lastTimeStamp == 0) {
							lastTimeStamp = curTimeStamp;
						} else if (curTimeStamp - lastTimeStamp < 900) {
							lastTimeStamp = 0;
							ptTeleport = fingerXY[sdlEvent.tfinger.fingerId];
						} else {
							lastTimeStamp = 0;
						}
					}
				} else {
					processEvent(Mouse, true, SDL_BUTTON_LEFT);
				}
			}
			break;
		motion:
		case SDL_FINGERMOTION:
			if (sdlEvent.tfinger.fingerId >= 5) {
				break;
			} else {
				if (gameState->Begun && !gameState->Paused && !gameState->Charstats)
				{
					SDL_Rect viewport;
					glGetIntegerv(GL_VIEWPORT, (GLint*)&viewport);
					fingerXY[sdlEvent.tfinger.fingerId].x = int(sdlEvent.tfinger.x * viewport.w);
					fingerXY[sdlEvent.tfinger.fingerId].y = int(sdlEvent.tfinger.y * viewport.h);
					SDL_Rect rc;
					if (hud->isPointInRect(violetland::HUD::AREA_SHOOT, fingerXY[sdlEvent.tfinger.fingerId], &rc)
						|| fingerIdShoot == sdlEvent.tfinger.fingerId)
					{
						isFiring = true;
						fingerIdShoot = sdlEvent.tfinger.fingerId;

						int x = fingerXY[sdlEvent.tfinger.fingerId].x;
						int y = fingerXY[sdlEvent.tfinger.fingerId].y;
						if (x < rc.x)
							x = rc.x;
						else if (x > rc.x + rc.w)
							x = rc.x + rc.w;
						if (y < rc.y)
							y = rc.y;
						else if (y > rc.y + rc.h)
							y = rc.y + rc.h;

						double relX = x - (rc.x + rc.w/2.0);
						double relY = y - (rc.y + rc.h/2.0);
						double distance_scr = hypot(relY, relX);
						double distance_x = distance_scr / (rc.w/2.0) * cam->getHalfW();
						double distance_y = distance_scr / (rc.h/2.0) * cam->getHalfH();
						double angle = atan2(relY, relX);

						fingerXY[sdlEvent.tfinger.fingerId].x = -1;
						fingerXY[sdlEvent.tfinger.fingerId].y = -1;

						target.x = (int)round(distance_x * cos(angle));
						target.y = (int)round(distance_y * sin(angle));

					} else if (hud->isPointInRect(violetland::HUD::AREA_WASD, fingerXY[sdlEvent.tfinger.fingerId], NULL)) {
						fingerIdWasd = sdlEvent.tfinger.fingerId;
					}
				}
			}
			break;
#endif
		case SDL_QUIT:
			g_isExit = true;
			m_event[Exit] = true;
			break;
		}
	}
}

string InputHandler::getEventName(int eventNumber) {
	if (eventNumber > GameInputEventsCount)
		throw exception();
	else
		return m_eventNames[eventNumber];
}

string InputHandler::getEventIdentifier(int eventNumber) {
	if (eventNumber > GameInputEventsCount)
		throw exception();
	else
		return m_eventIdentifiers[eventNumber];
}

const unsigned InputHandler::getEventNumber(std::string eventIdentifier) {
	for (unsigned i = 0; i < GameInputEventsCount; ++i)
		if (m_eventIdentifiers[i] == eventIdentifier)
			return i;

	throw exception();
}

string InputHandler::getKeyName(Binding bind) {
	if (bind.Type == InputHandler::Keyboard)
	{
		string keyName = SDL_GetKeyName(SDL_Keycode(bind.Value));
		return keyName;
	}
	else 
	{
		if (bind.Type == InputHandler::Mouse)
		{
			switch (bind.Value) {
			default:
			case SDL_BUTTON_LEFT:
				return "left mouse";
			case SDL_BUTTON_RIGHT:
				return "right mouse";
			case SDL_BUTTON_MIDDLE:
				return "middle mouse";
			}
		}
	}

	return NULL;
}

