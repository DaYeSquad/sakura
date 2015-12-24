/*
 * jni_helper.h
 *
 *  Created on: Jan 25, 2015
 *      Author: FrankLin
 */

#ifndef LESSCHATCORE_CORE_UTILS_ANDROID_JNI_HELPER_H_
#define LESSCHATCORE_CORE_UTILS_ANDROID_JNI_HELPER_H_

#include <jni.h>

#include <string>
#include <memory>
#include <vector>

#include "sakura/macros.h"

NS_SKR_BEGIN

typedef struct JniMethodInfo_ {
  JNIEnv* env;
  jclass class_id;
  jmethodID method_id;
} JniMethodInfo;

//JNI interface helper.
class JniHelper {
 public:

  // JavaVM & JNIEnv --------------------------------------------------------

  static void SetJavaVm(JavaVM* jvm);
  static JavaVM* GetJavaVm();

  static JNIEnv* GetJniEnv();

  static bool GetStaticMethodInfoOrDie(JniMethodInfo* methodinfo,
                                       const char* class_name,
                                       const char* method_name,
                                       const char* param_code);

  static bool GetMethodInfoOrDie(JniMethodInfo* methodinfo,
                                 const char* class_name,
                                 const char* method_name,
                                 const char* param_code);

  // Utils --------------------------------------------------------

  static std::string StringFromJstring(jstring str);

  static std::vector<std::string> StringVectorFromJobjectArray(jobjectArray jobjAry);

  static jobjectArray JobjectArrayFromStringVector(std::vector<std::string> str_vector);

  template<class T>
  static jlongArray JlongArrayFromNativeArray(std::vector<std::unique_ptr<T>> native_array) {
    size_t num_team = native_array.size();
    jlong* team_buf = reinterpret_cast<jlong*>(malloc(
        static_cast<unsigned long>(num_team * sizeof(jlong))));

    if (num_team > 0) {
      std::unique_ptr<T>* raw_teams = &native_array[0];
      for (int i = 0; i < num_team; ++i) {
        team_buf[i] = reinterpret_cast<jlong>(raw_teams[i].release());
      }
    }

    JNIEnv* env = GetJniEnv();

    jlongArray ret = env->NewLongArray(num_team);
    env->SetLongArrayRegion(ret, 0, num_team, team_buf);

    SKR_SAFE_DELETE(team_buf);
    return ret;
  }
  static bool IsUTF8(const char* content);

 private:
  static JNIEnv* CacheJniEnv(JavaVM* jvm);
};

NS_SKR_END

#endif /* LESSCHATCORE_CORE_UTILS_ANDROID_JNI_HELPER_H_ */
