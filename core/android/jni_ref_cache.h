/*
 * jni_ref_cache.h
 *
 *  Created on: May 21, 2015
 *      Author: FrankLin
 */

#ifndef LESSCORE_CORE_UTILS_ANDROID_JNI_REF_CACHE_H_
#define LESSCORE_CORE_UTILS_ANDROID_JNI_REF_CACHE_H_

#include "base/base.h"

#include "sakura/android/jni_helper.h"
#include "sakura/android/http_client_android.h"

NS_SKR_BEGIN

class JniReferenceCache {
 public:
  JniReferenceCache();
  virtual ~JniReferenceCache();

  static JniReferenceCache* SharedCache();
  static void PurgeCache();

  void CacheHttpClientCallbackMethod();
  void HandleCallbackOnMainThread(JNIEnv* env, HttpResponseCallback* callback);

 private:
  JniMethodInfo* callback_method_info_;

  DISALLOW_COPY_AND_ASSIGN(JniReferenceCache);
};

NS_SKR_END

#endif /* LESSCORE_CORE_UTILS_ANDROID_JNI_REF_CACHE_H_ */
