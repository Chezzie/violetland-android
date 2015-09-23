APP_STL := gnustl_static
APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti
APP_CPPFLAGS += -std=c++11
APP_ABI := armeabi armeabi-v7a x86
APP_CFLAGS += -DANDROID_USE_ONLY_GLES11
APP_PLATFORM := android-10
APP_CFLAGS += -Wno-error=format-security -Wno-format-security -Wno-attributes
