#include <jni.h>

#include "feed_message_manager.h"

extern  "C" {
JNIEXPORT jlong JNICALL
Java_com_sakura_test_FeedMessageManager_nativeCreateInstance(JNIEnv *env, jobject instance) {
  powertech::FeedMessageManager* manager = new powertech::FeedMessageManager();
  return reinterpret_cast<jlong>(manager);
}

JNIEXPORT void JNICALL
Java_com_sakura_test_FeedMessageManager_nativeConnect(JNIEnv *env, jobject instance, jlong handler,
                                                      jstring url_) {
  const char *url = env->GetStringUTFChars(url_, 0);

  powertech::FeedMessageManager* manager = reinterpret_cast<powertech::FeedMessageManager*>(handler);
  manager->Connect(url);

  env->ReleaseStringUTFChars(url_, url);
}
}

