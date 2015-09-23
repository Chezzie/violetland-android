#include "Configuration.h"
#include <JNIBridge.h>

Configuration::Configuration(FileUtility* fileUtility) {

	m_fileUtility = fileUtility;

#ifdef __ANDROID__
	JNIBridge::singleton().getScreenSize(&Screen.Width, &Screen.Height);
	Screen.Full = true;
#else
	Screen.Width = 800;
	Screen.Height = 600;
	Screen.Full = false;
#endif

	Screen.Color = 16;

	FrameDelay = 15;
	ShowFps = false;
	AutoReload = true;
	SoundVolume = 4;
	MusicVolume = 4;
	MonstersAtStart = 12;
	AimColorA = 0x000000;
	AimColorB = 0xFFFFFF;
	AutoWeaponPickup = true;
	FriendlyFire = false;

	PlayerInputBinding[InputHandler::Teleport].Value = SDLK_q;
	PlayerInputBinding[InputHandler::MoveLeft].Value = SDLK_a;
	PlayerInputBinding[InputHandler::MoveUp].Value = SDLK_w;
	PlayerInputBinding[InputHandler::MoveRight].Value = SDLK_d;
	PlayerInputBinding[InputHandler::MoveDown].Value = SDLK_s;
#ifdef __ANDROID__
	PlayerInputBinding[InputHandler::Restart].Value = SDLK_AC_BACK;
	PlayerInputBinding[InputHandler::Menu].Value = SDLK_AC_BACK;
#else
	PlayerInputBinding[InputHandler::Restart].Value = SDLK_RETURN;
	PlayerInputBinding[InputHandler::Menu].Value = SDLK_ESCAPE;
#endif
	PlayerInputBinding[InputHandler::MenuClickA].Type = InputHandler::Mouse;
	PlayerInputBinding[InputHandler::MenuClickA].Value = SDL_BUTTON_LEFT;
	PlayerInputBinding[InputHandler::MenuClickB].Type = InputHandler::Mouse;
	PlayerInputBinding[InputHandler::MenuClickB].Value = SDL_BUTTON_RIGHT;
	PlayerInputBinding[InputHandler::Exit].Value = SDLK_F12;
	PlayerInputBinding[InputHandler::ToggleLight].Value = SDLK_f;
	PlayerInputBinding[InputHandler::ToggleLaser].Value = SDLK_g;
	PlayerInputBinding[InputHandler::Pause].Value = SDLK_p;
	PlayerInputBinding[InputHandler::ShowChar].Value = SDLK_c;
	PlayerInputBinding[InputHandler::Help].Value = SDLK_F1;
	PlayerInputBinding[InputHandler::Pickup].Value = SDLK_e;
#ifdef __ANDROID__
	PlayerInputBinding[InputHandler::ThrowGrenade].Value = SDLK_g;
#else
	PlayerInputBinding[InputHandler::ThrowGrenade].Value = SDLK_SPACE;
#endif
	PlayerInputBinding[InputHandler::Fire].Value = SDL_BUTTON_LEFT;
	PlayerInputBinding[InputHandler::Fire].Type = InputHandler::Mouse;
#ifdef __ANDROID__
	PlayerInputBinding[InputHandler::Reload].Value = SDLK_r;
#else
	PlayerInputBinding[InputHandler::Reload].Value = SDL_BUTTON_RIGHT;
	PlayerInputBinding[InputHandler::Reload].Type = InputHandler::Mouse;
#endif
}

void Configuration::read() {
#ifndef __ANDROID__
	try {
		ConfigFile cFile(
				m_fileUtility->getFullPath(FileUtility::user, "config"));
		LOG("Config file opened successfully");
		cFile.readInto(Screen.Width, "screenWidth");
		cFile.readInto(Screen.Height, "screenHeight");
		cFile.readInto(Screen.Color, "screenColor");
		cFile.readInto(Screen.Full, "fullScreen");
		cFile.readInto(FrameDelay, "frameDelay");
		cFile.readInto(ShowFps, "showFps");
		cFile.readInto(AutoReload, "autoReload");
		cFile.readInto(SoundVolume, "soundVolume");
		cFile.readInto(MusicVolume, "musicVolume");
		cFile.readInto(AimColorA, "aimColorA");
		cFile.readInto(AimColorB, "aimColorB");
		cFile.readInto(AutoWeaponPickup, "autoWeaponPickup");
		cFile.readInto(FriendlyFire, "friendlyFire");

		for (int i = 0; i < InputHandler::GameInputEventsCount; i++) {
			ReadPlayerBinding(&cFile, &PlayerInputBinding[i],
					InputHandler::getEventIdentifier(i));
		}

	} catch (...) {
		LOG("Can't open config file.");
		// std::cout << "Can't open config file." << std::endl;
	}
#endif
}

void Configuration::ReadPlayerBinding(ConfigFile* cFile,
		InputHandler::Binding* binding, std::string eventIdentifier) {
	int type;
	std::string keyType = "playerInputBinding_" + eventIdentifier + "Type";
	std::string keyValue = "playerInputBinding_" + eventIdentifier + "Value";

	if (cFile->keyExists(keyType)) {
		cFile->readInto(type, keyType);
		binding->Type = (InputHandler::BindingType) type;
		cFile->readInto(binding->Value, keyValue);
	}
}

void Configuration::WritePlayerBinding(ConfigFile* cFile,
		InputHandler::Binding* binding, std::string eventIdentifier) {
	std::string keyType = "playerInputBinding_" + eventIdentifier + "Type";
	std::string keyValue = "playerInputBinding_" + eventIdentifier + "Value";

	cFile->add(keyType, (int) binding->Type);
	cFile->add(keyValue, binding->Value);
}

void Configuration::write() {
#ifndef __ANDROID__
	ConfigFile cFile;

	cFile.add("aimColorB", AimColorB);
	cFile.add("aimColorA", AimColorA);
	cFile.add("soundVolume", SoundVolume);
	cFile.add("musicVolume", MusicVolume);
	cFile.add("autoReload", AutoReload);
	cFile.add("showFps", ShowFps);
	cFile.add("frameDelay", FrameDelay);
	cFile.add("fullScreen", Screen.Full);
	cFile.add("screenColor", Screen.Color);
	cFile.add("screenHeight", Screen.Height);
	cFile.add("screenWidth", Screen.Width);
	cFile.add("autoWeaponPickup", AutoWeaponPickup);
	cFile.add("friendlyFire", FriendlyFire);

	for (int i = 0; i < InputHandler::GameInputEventsCount; i++) {
		WritePlayerBinding(&cFile, &PlayerInputBinding[i],
				InputHandler::getEventIdentifier(i));
	}

	boost::filesystem::ofstream ofile(
			m_fileUtility->getFullPath(FileUtility::user, "config"));
	if (ofile) {
		ofile << cFile;
		ofile.close();
	}
#endif
}

Configuration::~Configuration() {
	// nothing
}
