
//
//  log.m
//  LessChatCore
//
//  Created by Frank Lin on 11/19/14.
//
//

#include <stdarg.h>
#include <stdio.h>

#include "sakura/log.h"

NS_SKR_BEGIN

void log_event(const char*format, ... ) {
#if SKR_LOG_LEVEL == SKR_LOG_LEVEL_ALL
  printf("log : ");
  char buf[kMaxLogLen+1] = {0};
  va_list ap;
  va_start(ap, format);
  vsnprintf(buf, kMaxLogLen, format, ap);
  va_end(ap);
  printf("%s", buf);
  printf("\n");
#endif
}

void log_warning(const char*format, ... ) {
#if SKR_LOG_LEVEL == SKR_LOG_LEVEL_ALL || SKR_LOG_LEVEL_ == SKR_LOG_LEVEL_WARNING
  printf("warning : ");
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
#if SKR_LOG_LEVEL == SKR_LOG_LEVEL_ALL || SKR_LOG_LEVEL_ == SKR_LOG_LEVEL_WARNING || SKR_LOG_LEVEL == SKR_LOG_LEVEL_ERROR
  printf("error : ");
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
