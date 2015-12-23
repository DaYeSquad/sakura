//
//  FileUtilsiOS.h
//  OneMap SDK for iOS-X
//
//  Created by Frank Lin on 13-10-18.
//  Copyright (c) 2013年 Frank Lin. All rights reserved.
//

#ifndef LESSCHATCORE_CORE_UTILS_IOS_FILE_UTILS_WIN32_H_
#define LESSCHATCORE_CORE_UTILS_IOS_FILE_UTILS_WIN32_H_

#include <iostream>

#include "base/base.h"
#include "utils/file_utils.h"

NS_LCC_BEGIN

class FileUtilsWin32 : public lesschat::FileUtils{
  
public:
  
  // Utilities (overrides) --------------------------------------------------------

  virtual bool IsFileExist(const std::string& file_path) OVERRIDE;

  virtual const char* PreferredDocumentPath() OVERRIDE;

	virtual const char* PreferredCachePath() OVERRIDE;
  
  virtual bool CreateFolderAtPath(const char* path) OVERRIDE;
};

NS_LCC_END

#endif /* defined(LESSCHATCORE_CORE_UTILS_IOS_FILE_UTILS_WIN32_H_) */
