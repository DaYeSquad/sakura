/*
 * jni_ref_cache.cc
 *
 *  Created on: May 21, 2015
 *      Author: FrankLin
 */

#include "jni_ref_cache.h"
#include "jni_helper.h"

USING_NS_SKR;

using std::string;

static JniReferenceCache* shared_cache_ = nullptr;

JniReferenceCache::JniReferenceCache()
    : callback_method_info_(nullptr) {
}

JniReferenceCache::~JniReferenceCache() {

}

JniReferenceCache* JniReferenceCache::SharedCache() {
  if (shared_cache_ == nullptr) {
    shared_cache_ = new JniReferenceCache();
  }
  return shared_cache_;
}

void JniReferenceCache::PurgeCache() {
  WTC_SAFE_DELETE(shared_cache_);
}

void JniReferenceCache::CacheHttpClientCallbackMethod() {
  JniMethodInfo* method_info = new JniMethodInfo();
  bool success = JniHelper::GetStaticMethodInfoOrDie(
      method_info, "com/lesschat/core/jni/HttpClientJniHelper",
      "HandleCallbackOnMainThread", "(J)V");
  method_info->class_id = reinterpret_cast<jclass>(method_info->env->NewGlobalRef(method_info->class_id));
  callback_method_info_ = method_info;
}

void JniReferenceCache::HandleCallbackOnMainThread(JNIEnv* env,
    HttpResponseCallback* callback) {
  jlong pointer = reinterpret_cast<jlong>(callback);
  env->CallStaticVoidMethod(
      callback_method_info_->class_id, callback_method_info_->method_id,
      pointer);
}
