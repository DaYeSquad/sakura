/*
 * FileUtilsAndroid.cpp
 *
 *  Created on: 2013-9-11
 *      Author: FrankLin
 */

#include "file_utils_android.h"

#include <stdio.h>

#include "sakura/log.h"
#include "sakura/file_utils.h"

using std::string;

NS_SKR_BEGIN

////////////////////////////////////////////////////////////////////////////////
// FileUtilsAndroid, public:

// Creation and lifetime --------------------------------------------------------

FileUtils* FileUtils::SharedFileUtils() {
  if (s_shared_fileutil_ == nullptr) {
    s_shared_fileutil_ = new FileUtilsAndroid();
  }

  return s_shared_fileutil_;
}

FileUtilsAndroid::~FileUtilsAndroid() {
}

// Overrides --------------------------------------------------------

bool FileUtilsAndroid::IsFileExist(const std::string& filepath) {

  if (0 == filepath.length()) {
    return false;
  }

  bool found = false;

  //TODO(Frank, lin.xiaoe.f@gmail.com) : Check whether file exists in apk.

  FILE *fp = fopen(filepath.c_str(), "r");
  if (fp) {
    found = true;
    fclose(fp);
  }

  return found;
}

const char* FileUtilsAndroid::PreferredDocumentPath() {
  //TODO(Frank, lin.xiaoe.f@gmail.com) : NOT IMPLEMENTED YET.
#ifdef WTC_UNITTEST
  return "/data/data/com.lesschat.unittest/files/";
#else
  return "";
#endif
}

const char* FileUtilsAndroid::PreferredCachePath() {
  return nullptr;
}

bool FileUtilsAndroid::CreateFolderAtPath(const char* path) {
  //TODO(Frank, lin.xiaoe.f@gmail.com) : NOT IMPLEMENTED YET.
  log_error("LessChat core doesn't implment this method");
  return false;
}

NS_SKR_END