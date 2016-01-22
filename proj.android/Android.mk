LOCAL_PATH := $(call my-dir)/../sakura

include $(CLEAR_VARS)

COMMON  := $(wildcard $(LOCAL_PATH)/*.cc)
ANDROID := $(wildcard $(LOCAL_PATH)/android/*.cc)

LOCAL_MODULE := sakura
LOCAL_SRC_FILES += $(COMMON:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(ANDROID:$(LOCAL_PATH)/%=%)

LOCAL_CPPFLAGS  += -fexceptions
LOCAL_CPPFLAGS  += -D__GXX_EXPERIMENTAL_CXX0X__ -D__STDC_CONSTANT_MACROS -DSKR_PLATFORM=SKR_PLATFORM_ANDROID -DLCC_THREAD_SAFE -DHAVE_INTTYPES_H

LOCAL_C_INCLUDES += $(LOCAL_PATH)/.. \
					$(LOCAL_PATH)/../external/curl/curl/android/curl \
					$(LOCAL_PATH)/../external \
					$(LOCAL_PATH)/../external/openssl/include \
					$(LOCAL_PATH)/../external/websockets/include/android 

include $(BUILD_STATIC_LIBRARY)