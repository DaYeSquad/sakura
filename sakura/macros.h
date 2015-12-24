//
//  macros.h
//  WorktileCore
//
//  Created by Frank Lin on 10/29/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#ifndef SAKURA_MACROS_H_
#define SAKURA_MACROS_H_

// DISALLOW_COPY_AND_ASSIGN (google magic)
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName&); \
void operator=(const TypeName&)

// namespace sakura {}
#ifdef __cplusplus
#define NS_SKR_BEGIN                  namespace sakura {
#define NS_SKR_END                    }
#define USING_NS_SKR                  using namespace sakura
#else
#define NS_SKR_BEGIN
#define NS_SKR_END
#define USING_NS_SKR
#endif

// delete/free
#define SKR_SAFE_DELETE(p) do { if(p) { delete (p); (p) = nullptr; } } while(0)
#define SKR_SAFE_DELETE_ARRAY(p) do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define SKR_SAFE_FREE(p) do { if(p) { free(p); (p) = 0; } } while(0)
#define SKR_SAFE_RELEASE(p) do { if(p) { (p)->Release(); (p)=nullptr; } } while(0)

// break if
#define SKR_BREAK_IF(XXXX) if(XXXX)break;

// platform enum
#define SKR_PLATFORM_UNKNOWN            0
#define SKR_PLATFORM_IOS                1
#define SKR_PLATFORM_ANDROID            2
#define SKR_PLATFORM_MAC                3
#define SKR_PLATFORM_WIN32              5

// SKR_DLL
#if SKR_PLATFORM == SKR_PLATFORM_WIN32
#if defined(SKR_STATIC)
#define SKR_DLL
#else
#if defined(_USRDLL)
#define SKR_DLL     __declspec(dllexport)
#else
#define SKR_DLL     __declspec(dllimport)
#endif
#endif
#else
#define SKR_DLL
#endif // SKR_PLATFORM == SKR_PLATFORM_WIN32

// log level enum
#define SKR_LOG_LEVEL_ALL     0
#define SKR_LOG_LEVEL_WARNING 1
#define SKR_LOG_LEVEL_ERROR   2

// assert
#ifdef SKR_USING_ASSERT
#include <assert.h>
#define SKR_ASSERT(p) assert(p)
#else
#define SKR_ASSERT(p)
#endif

// Annotate a virtual method indicating it must be overriding a virtual
// method in the parent class.
// Use like:
//   virtual void foo() OVERRIDE;
#if defined(COMPILER_MSVC)
#define OVERRIDE override
#elif defined(__clang__)
#define OVERRIDE override
#elif defined(COMPILER_GCC) && __cplusplus >= 201103 && \
(__GNUC__ * 10000 + __GNUC_MINOR__ * 100) >= 40700
// GCC 4.7 supports explicit virtual overrides when C++11 support is enabled.
#define OVERRIDE override
#else
#define OVERRIDE
#endif

#endif /* SAKURA_MACROS_H_ */
