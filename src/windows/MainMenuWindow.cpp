// #include <libintl.h>
#include <Translation.h>
#include <locale.h>
#include <JNIBridge.h>
#include "MainMenuWindow.h"
#define _(STRING)            gettext(STRING)

using namespace violetland;
using namespace std;

MainMenuWindow::MainMenuWindow(Configuration* config, GameState* gameState,
		TextManager* text) :
	Window(0.0f, 0.0f, config->Screen.Width, config->Screen.Height, 0.0f, 0.0f,
			0.0f, 0.5f) {

	m_gameState = gameState;

	const int l = config->Screen.Width * 0.1f;
	const int h = text->getHeight();

	std::vector<Label> labels;
	if (m_gameState->Begun && !m_gameState->Lost)
		labels.push_back(Label("resume", _("Resume")));
	labels.push_back(Label("start", _("Start")));
	labels.push_back(Label("options", _("Options")));
	labels.push_back(Label("highscores", _("High scores")));
	labels.push_back(Label("about", _("About")));
	if (!JNIBridge::singleton().isKindle()) {
		labels.push_back(Label("shop", _("Purchase perk points")));
	}
	labels.push_back(Label("exit", _("Exit")));
	
	addElements(labels, text, 
			l, 4*h, h*1.6, TextManager::LEFT, TextManager::MIDDLE);

	addHandler(Window::hdl_lclick, "exit", onMenuItemClick);

}

void MainMenuWindow::exitGame()
{
	m_gameState->end();
}

void MainMenuWindow::onMenuItemClick(void* sender, string menuItem)
{
	MainMenuWindow* window = (MainMenuWindow*)sender;

	if (menuItem == "exit")
		window->exitGame();
}

MainMenuWindow::~MainMenuWindow() {

}
