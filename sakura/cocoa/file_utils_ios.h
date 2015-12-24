//
//  FileUtilsiOS.h
//  OneMap SDK for iOS-X
//
//  Created by Frank Lin on 13-10-18.
//  Copyright (c) 2013年 Frank Lin. All rights reserved.
//

#ifndef SAKURA_COCOA_FILE_UTILS_IOS_H_
#define SAKURA_COCOA_FILE_UTILS_IOS_H_

#include <iostream>

#include "sakura/macros.h"
#include "sakura/file_utils.h"

NS_SKR_BEGIN

// Implement some platform methods
class FileUtilsiOS : public FileUtils{
public:
  
  // Utilities (overrides) --------------------------------------------------------
  
  virtual std::string GetWritablePath() OVERRIDE;
};

NS_SKR_END

#endif /* defined(SAKURA_COCOA_FILE_UTILS_IOS_H_) */
