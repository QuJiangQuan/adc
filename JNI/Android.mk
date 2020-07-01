LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := hardcontrol
LOCAL_SRC_FILES := hardcontrol.c
LOCAL_LDLIBS += -llog 
LOCAL_LDLIBS +=-lm
LOCAL_PROGUARD_ENABLED:= disabled
include $(BUILD_SHARED_LIBRARY)