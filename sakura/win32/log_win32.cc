
//
//  log.m
//  LessChatCore
//
//  Created by Frank Lin on 11/19/14.
//
//

#include "sakura/log.h"

#include <stdarg.h>
#include <stdio.h>

#include <WinUser.h>
#include <Shlwapi.h>
#include <tchar.h>

static const int MAX_LOG_LENGTH = 16 * 1024;

NS_SKR_BEGIN

void log_event(const char*format, ... ) {
#if SKR_LOG_LEVEL == SKR_LOG_LEVEL_ALL
	va_list args;
	va_start(args, format);
	char buf[MAX_LOG_LENGTH];

	vsnprintf(buf, MAX_LOG_LENGTH - 3, format, args);
	strcat(buf, "\n");

	WCHAR wszBuf[MAX_LOG_LENGTH] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, buf, -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, buf, sizeof(buf), nullptr, FALSE);
	printf("%s", buf);
	fflush(stdout);
	va_end(args);
#endif
}

void log_warning(const char*format, ... ) {
#if SKR_LOG_LEVEL <= SKR_LOG_LEVEL_WARNING
    printf("Warning : ");
    char buf[kMaxLogLen+1] = {0};
    va_list ap;
    va_start(ap, format);
    vsnprintf(buf, kMaxLogLen, format, ap);
    va_end(ap);
    printf("%s", buf);
    printf("\n");
#endif
}

void log_error(const char*format, ... ) {
#if SKR_LOG_LEVEL <= SKR_LOG_LEVEL_ERROR
    printf("Error : ");
    char buf[kMaxLogLen+1] = {0};
    va_list ap;
    va_start(ap, format);
    vsnprintf(buf, kMaxLogLen, format, ap);
    va_end(ap);
    printf("%s", buf);
    printf("\n");
#endif
}

NS_SKR_END
