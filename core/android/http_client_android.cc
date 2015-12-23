/*
 * http_client_android.cc
 *
 *  Created on: May 12, 2015
 *      Author: FrankLin
 */

#include "http_client_android.h"

#include "sakura/http_client.h"
#include "sakura/android/jni_helper.h"
#include "sakura/android/jni_ref_cache.h"
#include "sakura/log.h"

USING_NS_SKR;

void HttpClient::HandleHttpResponseCallback(
    std::unique_ptr<HttpResponse> response,
    std::function<void(std::unique_ptr<HttpResponse>)> callback) {
//
//  HttpResponseCallback* response_callback = new HttpResponseCallback(
//      std::move(response), callback);
//
//  JavaVM* jvm = JniHelper::GetJavaVm();
//  JNIEnv *env;
//  jint ret = jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
//  jvm->AttachCurrentThread(&env, nullptr);
//
//  JniReferenceCache::SharedCache()->HandleCallbackOnMainThread(env ,response_callback);
//
//  jvm->DetachCurrentThread();
  callback(std::move(response));
}

