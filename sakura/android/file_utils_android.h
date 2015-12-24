/*
 * FileUtilsAndroid.h
 *
 *  Created on: 2013-9-11
 *      Author: FrankLin
 */

#ifndef SAKURA_ANDROID_FILE_UTILS_ANDROID_H_
#define SAKURA_ANDROID_FILE_UTILS_ANDROID_H_

#include "sakura/file_utils.h"

NS_SKR_BEGIN

class FileUtilsAndroid : public FileUtils {
 public:

  // Creation and lifetime --------------------------------------------------------

  virtual ~FileUtilsAndroid();

  // Overrides --------------------------------------------------------

  virtual bool IsFileExist(const std::string& filepath) OVERRIDE;

  virtual const char* PreferredDocumentPath() OVERRIDE;

  virtual const char* PreferredCachePath() OVERRIDE;

  virtual bool CreateFolderAtPath(const char* path) OVERRIDE;

  virtual std::string GetWritablePath() OVERRIDE { return ""; }
};

NS_SKR_END

#endif /* SAKURA_ANDROID_FILE_UTILS_ANDROID_H_ */
