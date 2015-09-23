#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

#include <string>

#include "SDL.h"

using namespace std;

class InputHandler {
public:
	enum BindingType {
		Keyboard = 0, Mouse
	};
	struct Binding {
	public:
		int Value;
		BindingType Type;

		Binding() {
			Type = Keyboard;
			Value = 0;
		}
	};
	enum GameInputEvents {
		Restart = 0,
		Exit,
		Menu,
		MenuClickA,
		MenuClickB,
		ToggleLight,
		ToggleLaser,
		ShowChar,
		Pause,
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Help,
		Pickup,
		ThrowGrenade,
		Fire,
		Reload,
		Teleport,
		GameInputEventsCount
	};
	enum InputMode {
		Direct = 0 // , Text, TextMandatory
	};
	InputHandler(Binding binding[]);
	void process();
	void setInputMode(InputMode mode);
	// void setInputModeText(bool mandatory, std::string text);
	const std::string& getTextToShow() const {
		return m_textContent;
	}
	bool hasBeenValidated() const {
		return m_textValidated;
	}
	bool getDownInput(GameInputEvents evnt) const {
		return m_event[evnt];
	}
	bool getPressInput(GameInputEvents evnt);
	void resetMouseButtons();
#ifdef __ANDROID__
	SDL_Point fingerXY[5];
	SDL_Point target;
	bool isFiring;
	int fingerIdWasd;
	int fingerIdShoot;
	int fingerIdChar;
	int fingerIdReload;
	int fingerIdGrenade;
	SDL_Point ptTeleport;
#else
	int mouseX, mouseY;
#endif
	static string getEventName(int eventNumber);
	static string getEventIdentifier(int eventNumber);
	static const unsigned getEventNumber(string eventIdentifier);
	static string getKeyName(Binding bind);
	static void initEventNames();
private:
	void processEvent(BindingType type, bool down, int value);
	// void processTextInput(SDL_Event sdlEvent);
	bool m_event[GameInputEventsCount];
	static string m_eventNames[GameInputEventsCount];
	static string m_eventIdentifiers[GameInputEventsCount];
	InputMode m_mode;
	bool m_textValidated;
	string m_textContent;
	int m_curTextPos;
	Binding* m_binding;
};

#endif /* INPUTHANDLER_H_ */
