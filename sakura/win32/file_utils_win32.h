//
//  FileUtilsiOS.h
//  OneMap SDK for iOS-X
//
//  Created by Frank Lin on 13-10-18.
//  Copyright (c) 2013年 Frank Lin. All rights reserved.
//

#ifndef SAKURA_WIN32_FILE_UTILS_WIN32_H_
#define SAKURA_WIN32_FILE_UTILS_WIN32_H_

#include <iostream>

#include "sakura/macros.h"
#include "sakura/file_utils.h"

NS_SKR_BEGIN

class FileUtilsWin32 : public FileUtils{
  
public:
  
  // Utilities (overrides) --------------------------------------------------------

	virtual std::string GetWritablePath() OVERRIDE;

  virtual bool IsFileExist(const std::string& path) OVERRIDE;
};

NS_SKR_END

#endif /* defined(SAKURA_WIN32_FILE_UTILS_WIN32_H_) */
