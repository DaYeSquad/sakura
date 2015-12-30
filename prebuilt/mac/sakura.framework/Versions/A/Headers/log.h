//
//  log.h
//  LessChatCore
//
//  Created by Frank Lin on 11/19/14.
//
//

#ifndef SAKURA_LOG_H_
#define SAKURA_LOG_H_

#include "sakura/macros.h"

NS_SKR_BEGIN

static const int kMaxLogLen = 16 * 1024;

SKR_DLL void log_event(const char*format, ...);
SKR_DLL void log_warning(const char*format, ...);
SKR_DLL void log_error(const char*format, ...);

NS_SKR_END

#endif /* defined(SAKURA_LOG_H_) */
