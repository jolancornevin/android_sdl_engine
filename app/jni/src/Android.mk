LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../SDL/include $(LOCAL_PATH)/../SDL2_image $(LOCAL_PATH)/../header

# Add your application source files here...
LOCAL_SRC_FILES := loop.cpp

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
