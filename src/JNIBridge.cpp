#ifdef __ANDROID__

#include "JNIBridge.h"
#include <android/log.h>

JavaVM* g_pJavaVM;
static JNIBridge* instance = NULL;

extern "C" jint JNI_OnLoad(JavaVM* jvm, void *reserved)
{
	g_pJavaVM = jvm;
	JNIEnv* pEnv = 0;
	if (jvm->GetEnv((void**)&pEnv, JNI_VERSION_1_2))
		return JNI_ERR;
	return JNI_VERSION_1_2;
}

JNIBridge::JNIBridge()
{
    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);

    jclass jc = pEnv->FindClass("com/xteleport/violetland/MainActivity");
    m_class = (jclass)pEnv->NewGlobalRef(jc);
    pEnv->DeleteLocalRef(jc);

    m_getPath = pEnv->GetStaticMethodID(m_class, "getDataPath", "()Ljava/lang/String;");
    m_getScreenSize = pEnv->GetStaticMethodID(m_class, "getScreenSize", "()J");
    m_mmToPx = pEnv->GetStaticMethodID(m_class, "mmToPx", "(I)I");
    m_showAbout = pEnv->GetStaticMethodID(m_class, "showAbout", "()V");
    m_purchasePoints = pEnv->GetStaticMethodID(m_class, "purchasePoints", "()V");
    m_decreasePurchasedPoints = pEnv->GetStaticMethodID(m_class, "decreasePurchasedPoints", "()V");
    m_getPurchasedPoints = pEnv->GetStaticMethodID(m_class, "getPurchasedPoints", "()I");
    m_showBanner = pEnv->GetStaticMethodID(m_class, "showBanner", "(Z)V");
    m_showInterstitial = pEnv->GetStaticMethodID(m_class, "showInterstitial", "()V");
    m_showLeaderboard = pEnv->GetStaticMethodID(m_class, "showLeaderboard", "(Z)Z");
    m_reportScore = pEnv->GetStaticMethodID(m_class, "reportScore", "(IZ)V");
    m_isKindle = pEnv->GetStaticMethodID(m_class, "isKindle", "()Z");
}

JNIBridge::~JNIBridge()
{
    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);
	pEnv->DeleteGlobalRef(m_class);
}

JNIBridge& JNIBridge::singleton()
{
	if (instance == NULL)
		instance = new JNIBridge();
	return *instance;
}

void JNIBridge::reset() {
	if (instance != NULL) {
		delete instance;
		instance = NULL;
	}
}

std::string JNIBridge::getDataPath()
{
	std::string result;

    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);

	jstring path = (jstring)pEnv->CallStaticObjectMethod(m_class, m_getPath);
    const char* str = pEnv->GetStringUTFChars(path, NULL);
    result = str;
    pEnv->ReleaseStringUTFChars(path, str);
    pEnv->DeleteLocalRef(path);

    return result;
}

void JNIBridge::getScreenSize(int* width, int* height)
{
    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);
    long long result = pEnv->CallStaticLongMethod(m_class, m_getScreenSize);
    *width = int(result >> 32);
    *height = int(result & 0xFFFFFFFF);
}

int JNIBridge::mmToPx(int mm)
{
    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);
    int result = pEnv->CallStaticIntMethod(m_class, m_mmToPx, (jint)mm);
    return result;
}

void JNIBridge::showAbout()
{
    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);
    pEnv->CallStaticVoidMethod(m_class, m_showAbout);
}

void JNIBridge::purchasePoints()
{
    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);
    pEnv->CallStaticVoidMethod(m_class, m_purchasePoints);
}

int JNIBridge::getPurchasedPoints()
{
    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);
    jint result = pEnv->CallStaticIntMethod(m_class, m_getPurchasedPoints);
    return (int)result;
}

void JNIBridge::decreasePurchasedPoints()
{
    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);
    pEnv->CallStaticVoidMethod(m_class, m_decreasePurchasedPoints);
}

void JNIBridge::showBanner(bool show)
{
    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);
    pEnv->CallStaticVoidMethod(m_class, m_showBanner, show ? JNI_TRUE : JNI_FALSE);
}

void JNIBridge::showInterstitial()
{
    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);
    pEnv->CallStaticVoidMethod(m_class, m_showInterstitial);
}

bool JNIBridge::showLeaderboard(bool attackWaves)
{
    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);
    jboolean result = pEnv->CallStaticBooleanMethod(m_class, m_showLeaderboard, attackWaves ? JNI_TRUE : JNI_FALSE);
    return result == JNI_TRUE;
}

void JNIBridge::reportScore(int xp, bool attackWaves)
{
    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);
    pEnv->CallStaticVoidMethod(m_class, m_reportScore, (jint)xp, attackWaves ? JNI_TRUE : JNI_FALSE);
}

bool JNIBridge::isKindle()
{
    JNIEnv* pEnv;
    g_pJavaVM->GetEnv((void**)&pEnv, JNI_VERSION_1_2);
    jboolean result = pEnv->CallStaticBooleanMethod(m_class, m_isKindle);
    return result == JNI_TRUE;
}

#endif
