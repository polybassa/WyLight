LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
FW_SRC := ../../../firmware/
LIB_SRC := ../../../library/
LOCAL_LDLIBS := -llog
LOCAL_MODULE := wifly
LOCAL_SRC_FILES := $(FW_SRC)crc.c
LOCAL_SRC_FILES += $(LIB_SRC)BroadcastReceiver.cpp
LOCAL_SRC_FILES += $(LIB_SRC)ClientSocket.cpp
LOCAL_SRC_FILES += $(LIB_SRC)ComProxy.cpp
LOCAL_SRC_FILES += $(LIB_SRC)intelhexclass.cpp
LOCAL_SRC_FILES += $(LIB_SRC)MaskBuffer.cpp
LOCAL_SRC_FILES += $(LIB_SRC)Script.cpp
LOCAL_SRC_FILES += $(LIB_SRC)ScriptManager.cpp
LOCAL_SRC_FILES += $(LIB_SRC)StartupManager.cpp
LOCAL_SRC_FILES += $(LIB_SRC)TelnetProxy.cpp
LOCAL_SRC_FILES += $(LIB_SRC)WiflyControl.cpp
LOCAL_SRC_FILES += $(LIB_SRC)WiflyControlNoThrow.cpp
LOCAL_SRC_FILES += WiflyControlJni.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(FW_SRC)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(LIB_SRC)

include $(BUILD_SHARED_LIBRARY)
