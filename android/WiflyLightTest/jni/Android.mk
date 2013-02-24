LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
DIR := ../../../
LOCAL_LDLIBS := -llog
LOCAL_CFLAGS := -DX86 -DOS_ANDROID
LOCAL_MODULE := wifly
LOCAL_SRC_FILES += JniTestWrapper.cpp
LOCAL_SRC_FILES += $(DIR)WiflyControlJni.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(DIR)c++11_wrapper
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(DIR)
include $(BUILD_SHARED_LIBRARY)
