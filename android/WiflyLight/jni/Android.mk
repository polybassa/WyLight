LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS := -llog
LOCAL_CFLAGS := -DX86 -DANDROID
LOCAL_MODULE := wifly
LOCAL_SRC_FILES := ../../../ClientSocket.cpp

include $(BUILD_SHARED_LIBRARY)
