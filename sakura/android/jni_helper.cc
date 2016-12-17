/*
 * jni_helper.cc
 *
 *  Created on: Jan 25, 2015
 *      Author: FrankLin
 */

#include <pthread.h>
#include "jni_helper.h"

#include "sakura/log.h"

NS_SKR_BEGIN

////////////////////////////////////////////////////////////////////////////////
// JniHelper, C:

static pthread_key_t k_jvm;

void detach_current_thread(void* parameter) {
  JniHelper::GetJavaVm()->DetachCurrentThread();
}

////////////////////////////////////////////////////////////////////////////////
// JniHelper, public:

// JavaVM & JNIEnv --------------------------------------------------------

static JavaVM* shared_java_vm_ = nullptr;

std::string JniHelper::StringFromJstring(jstring jstr) {
  if (jstr == nullptr) {
    return "";
  }

  JNIEnv* env = GetJniEnv();
  if (!env) {
    return nullptr;
  }

  const char* chars = env->GetStringUTFChars(jstr, nullptr);
  std::string ret(chars);
  env->ReleaseStringUTFChars(jstr, chars);

  return ret;
}

std::vector<std::string> JniHelper::StringVectorFromJobjectArray(
    jobjectArray jobjAry) {

  std::vector<std::string> string_vector;

  if (jobjAry == nullptr) {
    return string_vector;
  }

  JNIEnv* env = GetJniEnv();
  if (!env) {
    return string_vector;
  }

  jsize size = env->GetArrayLength(jobjAry);

  for (int i = 0; i < size; i++) {
    jstring jstr = (jstring) env->GetObjectArrayElement(jobjAry, i);
    std::string str = StringFromJstring(jstr);
    string_vector.push_back(str);
    env->DeleteLocalRef(jstr);
  }
  return string_vector;
}

jobjectArray JniHelper::JobjectArrayFromStringVector(
    std::vector<std::string> str_vector) {
  jobjectArray jobjAry = 0;

  JNIEnv* env = GetJniEnv();
  if (!env) {
    return jobjAry;
  }

  jsize size = str_vector.size();
  if (size > 0) {
    jclass string_class = (env)->FindClass("java/lang/String");
    jobjAry = env->NewObjectArray(size, string_class, 0);
    for (int i = 0; i < size; i++) {
      std::string raw_member = str_vector[i];
      env->SetObjectArrayElement(jobjAry, i,
                                 env->NewStringUTF(raw_member.c_str()));
    }
  }
  return jobjAry;
}

void JniHelper::SetJavaVm(JavaVM* jvm) {
  shared_java_vm_ = jvm;

  pthread_key_create(&k_jvm, detach_current_thread);
}

JavaVM* JniHelper::GetJavaVm() {
  return shared_java_vm_;
}

JNIEnv* JniHelper::GetJniEnv() {
  JNIEnv* env = reinterpret_cast<JNIEnv*>(pthread_getspecific(k_jvm));
  if (env == nullptr) {
    env = CacheJniEnv(shared_java_vm_);
  }
  return env;
}

bool JniHelper::GetStaticMethodInfoOrDie(JniMethodInfo* methodinfo,
                                         const char* class_name,
                                         const char* method_name,
                                         const char* param_code) {
  if ((class_name == nullptr) || (method_name == nullptr)
      || (param_code == nullptr)) {
    return false;
  }

  JNIEnv* env = GetJniEnv();
  if (!env) {
    log_error("Failed to get JNIEnv");
    return false;
  }

  jclass class_id = env->FindClass(class_name);
  if (!class_id) {
    log_error("Failed to find class %s", class_name);
    return false;
  }

  jmethodID method_id = env->GetStaticMethodID(class_id, method_name,
                                               param_code);
  if (!method_id) {
    log_error("Failed to find static method %s", method_name);
    return false;
  }

  if (methodinfo) {
    methodinfo->class_id = class_id;
    methodinfo->env = env;
    methodinfo->method_id = method_id;
  }

  return true;
}

bool JniHelper::GetMethodInfoOrDie(JniMethodInfo* methodinfo,
                                   const char* class_name,
                                   const char* method_name,
                                   const char* param_code) {
  if ((class_name == nullptr) || (method_name == nullptr)
      || (param_code == nullptr)) {
    return false;
  }

  JNIEnv* env = GetJniEnv();
  if (!env) {
    log_error("Failed to get JNIEnv");
    return false;
  }

  jclass class_id = env->FindClass(class_name);
  if (!class_id) {
    log_error("Failed to find class %s", class_name);
    return false;
  }

  jmethodID method_id = env->GetMethodID(class_id, method_name, param_code);
  if (!method_id) {
    log_error("Failed to find method %s", method_name);
    return false;
  }

  if (methodinfo) {
    methodinfo->class_id = class_id;
    methodinfo->env = env;
    methodinfo->method_id = method_id;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// JniHelper, private:

JNIEnv* JniHelper::CacheJniEnv(JavaVM* jvm) {
  JNIEnv* env = nullptr;
  jint ret = jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

  switch (ret) {
    case JNI_OK:
      pthread_setspecific(k_jvm, env);
      return env;

    case JNI_EDETACHED: {
      //Thread not attached
      if (jvm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
        log_error("Failed to get the environment using AttachCurrentThread()");
        return nullptr;
      } else {
        //success to attach and obtain JNIEnv
        pthread_setspecific(k_jvm, env);
        return env;
      }
    }

    case JNI_EVERSION:
      log_error("JNI version 1.6 not supported");
      return nullptr;

    default:
      log_error("Failed to get the environment using GetEnv()");
      return nullptr;
  }
}

bool JniHelper::IsUTF8(const char* content) {
  long size = static_cast<long>(strlen(content));
  bool IsUTF8 = true;
  unsigned char* start = (unsigned char*) content;
  unsigned char* end = (unsigned char*) content + size;
  while (start < end) {
    if (*start < 0x80)  // (10000000): 值小于0×80的为ASCII字符
        {
      start++;
    } else if (*start < (0xC0))  // (11000000): 值介于0×80与0xC0之间的为无效UTF-8字符
        {
      IsUTF8 = false;
      break;
    } else if (*start < (0xE0))  // (11100000): 此范围内为2字节UTF-8字符
        {
      if (start >= end - 1)
        break;
      if ((start[1] & (0xC0)) != 0x80) {
        IsUTF8 = false;
        break;
      }
      start += 2;
    } else if (*start < (0xF0))  // (11110000): 此范围内为3字节UTF-8字符
        {
      if (start >= end - 2)
        break;
      if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80) {
        IsUTF8 = false;
        break;
      }
      start += 3;
    } else {
      IsUTF8 = false;
      break;
    }
  }
  return IsUTF8;
}

NS_SKR_END
