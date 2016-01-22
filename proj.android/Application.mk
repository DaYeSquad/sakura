APP_STL := gnustl_static
APP_PLATFORM := android-14
APP_CPPFLAGS := -std=c++11
APP_CPPFLAGS += -frtti #-Wno-format-security #-Wno-literal-suffix
APP_OPTIM := debug
NDK_TOOLCHAIN_VERSION=4.9
APP_ABI := armeabi armeabi-v7a x86