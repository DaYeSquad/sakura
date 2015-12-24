// FileUtils
// Inspired by cocos2d-x
// @author Frank Lin (lin.xiaoe.f@gmail.com)
//

#ifndef SAKURA_FILE_UTILS_H_
#define SAKURA_FILE_UTILS_H_

#include <string>
#include <vector>

#include "sakura/macros.h"

NS_SKR_BEGIN

//File utils
class SKR_DLL FileUtils {
public:
  
  // Creation and lifetime --------------------------------------------------------
  
  FileUtils();
  virtual ~FileUtils();
  
  //Shared instance
  static FileUtils* SharedFileUtils();
  static void PurgeFileUtils();
  
  // Utils --------------------------------------------------------
  
  virtual unsigned char* GetFileData(const char* filename, const char* mode,
                                     unsigned long* out_size);
  
  virtual bool IsDirectoryExist(const std::string& path);
  
  virtual bool CreateDirectory(const std::string& path);
  
  virtual bool RemoveDirectory(const std::string& path);
  
  virtual bool RemoveAllFilesInDirectory(const std::string& path);
  
  virtual bool RemoveAllFilesInDirectory(const std::string& path, const std::vector<std::string>& exclude_sub_dirs);
  
  virtual bool RemoveFile(const std::string& file);
  
  // Utils should be override on each platform --------------------------------------------------------
  
  virtual std::string GetWritablePath() = 0; // used for unit test
  
protected:
  static FileUtils* s_shared_fileutil_;
  
private:
  DISALLOW_COPY_AND_ASSIGN(FileUtils);
};

NS_SKR_END

#endif /* SAKURA_FILE_UTILS_H_ */
