LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
DIR := ../../../
LOCAL_LDLIBS := -llog
LOCAL_MODULE := wifly
LOCAL_SRC_FILES := $(DIR)BroadcastReceiver.cpp
LOCAL_SRC_FILES += $(DIR)ClientSocket.cpp
LOCAL_SRC_FILES += $(DIR)ComProxy.cpp
LOCAL_SRC_FILES += $(DIR)crc.c
LOCAL_SRC_FILES += $(DIR)intelhexclass.cpp
LOCAL_SRC_FILES += $(DIR)TelnetProxy.cpp
LOCAL_SRC_FILES += $(DIR)WiflyControl.cpp
LOCAL_SRC_FILES += $(DIR)WiflyControlColorClass.cpp
LOCAL_SRC_FILES += $(DIR)WiflyControlJni.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(DIR)c++11_wrapper

include $(BUILD_SHARED_LIBRARY)
