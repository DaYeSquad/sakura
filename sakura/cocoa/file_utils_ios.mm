//
//  FileUtilsiOS.cpp
//  OneMap SDK for iOS-X
//
//  Created by Frank Lin on 13-10-18.
//  Copyright (c) 2013年 Frank Lin. All rights reserved.
//

#include "file_utils_ios.h"

#import <Foundation/Foundation.h>

using std::string;
USING_NS_SKR;

////////////////////////////////////////////////////////////////////////////////
// FileUtilsiOS, public:

// Creation and lifetime --------------------------------------------------------

FileUtils* FileUtils::SharedFileUtils(){
  if (s_shared_fileutil_ == NULL) {
    
    s_shared_fileutil_ = new FileUtilsiOS();
  }
  return s_shared_fileutil_;
}

// Utility --------------------------------------------------------

std::string FileUtilsiOS::GetWritablePath() {
#if SKR_PLATFORM == SKR_PLATFORM_MAC
  NSError *error = nil;
  NSURL *app_support_dir = [[NSFileManager defaultManager] URLForDirectory:NSApplicationSupportDirectory
                                                                  inDomain:NSUserDomainMask
                                                         appropriateForURL:nil
                                                                    create:YES
                                                                     error:&error];
  return [app_support_dir UTF8String];
#elif SKR_PLATFORM == SKR_PLATFORM_IOS
  NSArray *searchPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
  NSString *documentPath = [searchPaths objectAtIndex:0];
  return [documentPath UTF8String];
#endif
}

bool FileUtilsiOS::IsFileExist(const std::string& path) {
  return [[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:path.c_str()]
                                              isDirectory:nil];
}
