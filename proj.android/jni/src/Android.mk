LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := main
LOCAL_ARM_MODE := arm
LOCAL_LDLIBS := -lGLESv1_CM -llog
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../../../../Packages/boost_1_53_0 \
    $(LOCAL_PATH)/../../../src

LOCAL_CFLAGS := -DHAVE_GLES -DGL_GLEXT_PROTOTYPES=1

#SDK_ROOT points to folder with SDL and oxygine-framework
LOCAL_SRC_FILES :=	../../../../../Oxygine/SDL/src/main/android/SDL_android_main.c

LOCAL_SRC_FILES += \
$(subst $(LOCAL_PATH)/,, \
 $(wildcard $(LOCAL_PATH)/../../../src/*.cpp) \
 $(wildcard $(LOCAL_PATH)/../../../src/game/*.cpp) \
 $(wildcard $(LOCAL_PATH)/../../../src/game/bullets/*.cpp) \
 $(wildcard $(LOCAL_PATH)/../../../src/game/lifeforms/*.cpp) \
 $(wildcard $(LOCAL_PATH)/../../../src/system/*.cpp) \
 $(wildcard $(LOCAL_PATH)/../../../src/system/graphic/*.cpp) \
 $(wildcard $(LOCAL_PATH)/../../../src/system/graphic/text/*.cpp) \
 $(wildcard $(LOCAL_PATH)/../../../src/system/sound/*.cpp) \
 $(wildcard $(LOCAL_PATH)/../../../src/system/utility/*.cpp) \
 $(wildcard $(LOCAL_PATH)/../../../src/windows/*.cpp) \
)

LOCAL_STATIC_LIBRARIES := oxygine-framework_static libboost_system libboost_filesystem
LOCAL_SHARED_LIBRARIES := libSDL2 SDL2_image SDL2_mixer SDL2_ttf

include $(BUILD_SHARED_LIBRARY)

#-----------------------------------------------------------
#import from NDK_MODULE_PATH defined in build.cmd
$(call import-add-path, ../../../Oxygine)
$(call import-module, oxygine-framework)

#-----------------------------------------------------------
$(call import-module, SDL2_image-2.0.0/external/libwebp-0.3.0)
$(call import-module, SDL2_image-2.0.0)

#-----------------------------------------------------------
$(call import-module, SDL2_mixer-2.0.0/external/libmikmod-3.1.12)
$(call import-module, SDL2_mixer-2.0.0/external/smpeg2-2.0.0)
$(call import-module, SDL2_mixer-2.0.0)

#-----------------------------------------------------------
$(call import-module, SDL2_ttf-2.0.12)

#-----------------------------------------------------------
$(call import-add-path, ../../../Packages/boost_1_53_0)
$(call import-module, lib-android)
