#pragma once

#ifdef __ANDROID__

#include <jni.h>
#include <string>

class JNIBridge
{
private:
	JNIBridge();
	~JNIBridge();

public:
	static JNIBridge& singleton();
	static void reset();

public:
	std::string getDataPath();
	void getScreenSize(int* width, int* height);
	int mmToPx(int mm);
	void showAbout();
	void purchasePoints();
	int getPurchasedPoints();
	void decreasePurchasedPoints();
	void showBanner(bool show);
	void showInterstitial();
	bool showLeaderboard(bool attackWaves);
	void reportScore(int xp, bool attackWaves);
	bool isKindle();

private:
	jclass m_class;
	jmethodID m_getPath;
	jmethodID m_getScreenSize;
	jmethodID m_mmToPx;
	jmethodID m_showAbout;
	jmethodID m_purchasePoints;
	jmethodID m_getPurchasedPoints;
	jmethodID m_decreasePurchasedPoints;
	jmethodID m_showBanner;
	jmethodID m_showInterstitial;
	jmethodID m_showLeaderboard;
	jmethodID m_reportScore;
	jmethodID m_showShop;
	jmethodID m_isKindle;
};

#endif

#if defined(__ANDROID__) && !defined(NDEBUG)
#include <android/log.h>
#define LOG(text) { __android_log_print(ANDROID_LOG_WARN, "VIOLET", text); }
#define LOGFMT(text, ...) { __android_log_print(ANDROID_LOG_WARN, "VIOLET", text, __VA_ARGS__); }
#define CHECK_GL_ERROR() { const int glerr = glGetError(); if (glerr != GL_NO_ERROR) __android_log_print(ANDROID_LOG_ERROR, "VIOLET", "glError: %d. (%s:%d))", glerr, __FILE__, __LINE__); }
#define CHECK_SDL_ERROR() { const char* sdlerr = SDL_GetError(); if (sdlerr != NULL) __android_log_print(ANDROID_LOG_ERROR, "VIOLET", "SDL_Error: %s. (%s:%d))", sdlerr, __FILE__, __LINE__); }
#define CHECK_TTF_ERROR() { const char* ttferr = TTF_GetError(); if (ttferr != NULL) __android_log_print(ANDROID_LOG_ERROR, "VIOLET", "TTF_Error: %s. (%s:%d))", ttferr, __FILE__, __LINE__); }
#define CHECKPOINT() { __android_log_print(ANDROID_LOG_INFO, "VIOLET", "CHECKPOINT. (%s:%d))", __FILE__, __LINE__); }
#else
#define LOG(text) {}
#define LOGFMT(text, ...) {}
#define CHECK_GL_ERROR() {}
#define CHECK_SDL_ERROR() {}
#define CHECK_TTF_ERROR() {}
#define CHECKPOINT() {}
#endif
