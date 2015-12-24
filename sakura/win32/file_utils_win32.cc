//
//  FileUtilsiOS.cpp
//  OneMap SDK for iOS-X
//
//  Created by Frank Lin on 13-10-18.
//  Copyright (c) 2013年 Frank Lin. All rights reserved.
//

#include "file_utils_win32.h"

#include <ShlObj.h>

#include "utils/log.h"

using std::string;
USING_NS_LCC;

////////////////////////////////////////////////////////////////////////////////
// FileUtilsiOS, public:

// Creation and lifetime --------------------------------------------------------

FileUtils* FileUtils::SharedFileUtils(){
  if (s_shared_fileutil_ == NULL) {
    
    s_shared_fileutil_ = new FileUtilsWin32();
  }
  return s_shared_fileutil_;
}

// Utility --------------------------------------------------------

bool FileUtilsWin32::IsFileExist(const string &file_path) {
	if (file_path.length() == 0) {
		return false;
	}
	
	const int max_path = 512;
	WCHAR utf16_buf[max_path] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, file_path.c_str(), -1, utf16_buf, sizeof(utf16_buf) / sizeof(utf16_buf[0]));

	DWORD attr = GetFileAttributesW(utf16_buf);
	if (attr = INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY)) {
		return false;
	}
	return true;
}

const char* FileUtilsWin32::PreferredDocumentPath() {
	log_error("LessChat core doesn't implement this method");
	return "";
}

const char* FileUtilsWin32::PreferredCachePath() {
	log_error("LessChat core doesn't implement this method");
	return "";
}

bool FileUtilsWin32::CreateFolderAtPath(const char* path) {
	log_error("LessChat core doesn't implement this method");
	return false;
}
