//
//  Log.m
//  OneMap SDK for iOS-X
//
//  Created by Frank Lin on 13-10-18.
//  Copyright (c) 2013年 Frank Lin. All rights reserved.
//

#include <stdarg.h>
#include <stdio.h>
#include <android/log.h>
#include <jni.h>
#include "sakura/log.h"

NS_SKR_BEGIN

#define LOG_TAG "LessChatCore C++"

void log(const char*format, ...) {

	va_list arg;
	va_start(arg, format);

	__android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, format, arg);

	va_end(arg);
}

void log_warning(const char*format, ...) {

	va_list arg;
	va_start(arg, format);

	__android_log_vprint(ANDROID_LOG_WARN, LOG_TAG, format, arg);

	va_end(arg);
}

void log_error(const char*format, ...) {

	va_list arg;
	va_start(arg, format);

	__android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, format, arg);

	va_end(arg);
}

void log_event(const char*format, ...) {

  va_list arg;
  va_start(arg, format);

  __android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, format, arg);

  va_end(arg);
}



NS_SKR_END
