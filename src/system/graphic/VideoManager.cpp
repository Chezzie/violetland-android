#include "VideoManager.h"

VideoManager::VideoManager(FileUtility* fileUtility) {
	m_fileUtility = fileUtility;

#ifdef WIN32
	 std::cout << "SDL_GL_SetAttribute SDL_GL_DOUBLEBUFFER..." << std::endl;
	 SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#endif

	RegularText = NULL;
	SmallText = NULL;

	// m_lastFrameTime = m_fpsCountingStart = SDL_GetTicks();

	// Seems that this code is supported only in windows.
	// std::cout << "SDL_GL_SetAttribute SDL_GL_SWAP_CONTROL..." << std::endl;
	// SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
}

bool VideoManager::isModeAvailable(int w, int h, int bpp, bool fullscreen, int* true_bpp) {
	/*Uint32 flags = SDL_OPENGL;
	if (fullscreen)
		flags = flags | SDL_FULLSCREEN;
	int r = SDL_VideoModeOK(w, h, bpp, flags);
	if (true_bpp)
		*true_bpp = r;
	return (r != 0);*/
	return true;
}

std::vector<SDL_Rect> VideoManager::GetAvailableModes() {
	/* This way is better than SDL_ListModes because of
	 * SDL_ListModes returns not all possible modes
	 */

	// Number of possible modes
	int wL[] = { 400, 640, 800, 1024, 1280, 1280, 1280, 1280, 1366, 1600, 1600,
			1680, 1920, 1920 };
	int hL[] = { 300, 480, 600, 768, 720, 768, 800, 1024, 768, 900, 1200, 1050,
			1080, 1200 };

	// If the mode is supported, it will be added to the return list
	std::vector<SDL_Rect> modes;
	for (unsigned int i = 0; i < getStructSize(wL); i++) {
		if (isModeAvailable(wL[i], hL[i], 16, true, NULL)) {
			SDL_Rect r;
			r.w = wL[i];
			r.h = hL[i];
			modes.push_back(r);
		}
	}

	return modes;
}

void VideoManager::setMode(VideoMode mode, Camera* cam) {
	std::cout << "SDL_SetVideoMode " << mode.Width << 'x' << mode.Height << '('
			<< (mode.Full ? 'f' : 'w') << ")..." << std::endl;

	m_videoMode = mode;

	/*SDL_Surface *screen = SDL_SetVideoMode(mode.Width, mode.Height, mode.Color,
			mode.Full ? SDL_OPENGL | SDL_FULLSCREEN : SDL_OPENGL);*/

	std::cout << "Calculating aspect size..." << std::endl;

	cam->setAspect((float) mode.Width / mode.Height);
	WK = (float) mode.Width / cam->getW();
	HK = (float) mode.Height / cam->getH();
	Scale = (float) mode.Width / 800;

	/* if (screen == NULL) {
		std::cerr << "Couldn't set video mode: " << SDL_GetError() << std::endl;
		exit(2);
	} */

	std::cout << "glViewport..." << std::endl;
	glViewport(0, 0, mode.Width, mode.Height);

	if (RegularText != NULL) {
		delete RegularText;
	}
	if (SmallText != NULL) {
		delete SmallText;
	}

	std::cout << "Preparing fonts..." << std::endl;
	boost::filesystem::path fontPath = m_fileUtility->getFullPath(
			FileUtility::common, "fonts/archangelsk.ttf");

	RegularText = new TextManager(fontPath, 46 * WK);
	SmallText = new TextManager(fontPath, 30 * WK);
}

VideoManager::~VideoManager() {
	delete RegularText;
	delete SmallText;
}
