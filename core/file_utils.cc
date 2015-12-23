/*
 * FileUtils.cpp
 *
 *  Created on: 2013-9-11
 *      Author: FrankLin
 */

#include "file_utils.h"

#include <sys/stat.h>
#include <vector>
#include <algorithm>

#if (SKR_PLATFORM == SKR_PLATFORM_WIN32)
#include <regex>
#include "sakura/win32/compact/dirent.h"
#endif

#if (SKR_PLATFORM == SKR_PLATFORM_IOS) || (SKR_PLATFORM == SKR_PLATFORM_MAC)
#include <ftw.h>
#endif

#if (SKR_PLATFORM != SKR_PLATFORM_WIN32)
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#endif

#include "sakura/log.h"

USING_NS_SKR;

////////////////////////////////////////////////////////////////////////////////
// FileUtils, C:

#if (SKR_PLATFORM == SKR_PLATFORM_IOS) || (SKR_PLATFORM == SKR_PLATFORM_MAC)
static int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
  auto ret = remove(fpath);
  if (ret) {
    log_event("Fail to remove:%s ",fpath);
  }
  
  return ret;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// FileUtils, public:

// Creation and lifetime --------------------------------------------------------

FileUtils *FileUtils::s_shared_fileutil_ = nullptr;

FileUtils::FileUtils() {
}

FileUtils::~FileUtils() {
}

void FileUtils::PurgeFileUtils() {
  SKR_SAFE_DELETE(s_shared_fileutil_);
}

// Utilities --------------------------------------------------------

unsigned char *FileUtils::GetFileData(const char *filename, const char *mode,
                                      unsigned long *out_size) {
  unsigned char *p_data = 0;
  
  if ((!filename) || (!mode) || 0 == strlen(filename)) {
    return 0;
  }
  
  do {
    FILE *fp = fopen(filename, mode);
    SKR_BREAK_IF(!fp);
    
    unsigned long size;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    p_data = new unsigned char[size + 1];
    size = fread(p_data, sizeof(unsigned char), size, fp);
    fclose(fp);
    
    p_data[size] = '\0';
    
    if (out_size) {
      *out_size = size;
    }
  }
  while (0);
  
  return p_data;
}

bool FileUtils::CreateDirectory(const std::string& path) {
  SKR_ASSERT(!path.empty());
  
  if (IsDirectoryExist(path))
    return true;
  
  // Split the path
  size_t start = 0;
  size_t found = path.find_first_of("/\\", start);
  std::string subpath;
  std::vector<std::string> dirs;
  
  if (found != std::string::npos)
  {
    while (true)
    {
      subpath = path.substr(start, found - start + 1);
      if (!subpath.empty())
        dirs.push_back(subpath);
      start = found+1;
      found = path.find_first_of("/\\", start);
      if (found == std::string::npos)
      {
        if (start < path.length())
        {
          dirs.push_back(path.substr(start));
        }
        break;
      }
    }
  }
#if (SKR_PLATFORM == SKR_PLATFORM_WIN32)
  if ((GetFileAttributesA(path.c_str())) == INVALID_FILE_ATTRIBUTES)
  {
    subpath = "";
    for (size_t i = 0; i < dirs.size(); ++i)
    {
      subpath += dirs[i];
      if (!IsDirectoryExist(subpath))
      {
        BOOL ret = CreateDirectoryA(subpath.c_str(), NULL);
        if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
        {
          return false;
        }
      }
    }
  }
  return true;
#else
  DIR *dir = NULL;
  
  // Create path recursively
  subpath = "";
  for (int i = 0; i < dirs.size(); ++i) {
    subpath += dirs[i];
    dir = opendir(subpath.c_str());
    if (!dir)
    {
      int ret = mkdir(subpath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
      if (ret != 0 && (errno != EEXIST))
      {
        return false;
      }
    }
  }
  return true;
#endif
}

bool FileUtils::IsDirectoryExist(const std::string& path) {
#if (SKR_PLATFORM == SKR_PLATFORM_WIN32)
	unsigned long fAttrib = GetFileAttributesA(path.c_str());
  if (fAttrib != INVALID_FILE_ATTRIBUTES &&
      (fAttrib & FILE_ATTRIBUTE_DIRECTORY))
  {
    return true;
  }
  return false;
#else
  struct stat st;
  if (stat(path.c_str(), &st) == 0)
  {
    return S_ISDIR(st.st_mode);
  }
  return false;
#endif
}

bool FileUtils::RemoveDirectory(const std::string& path) {
  if (path.size() > 0 && path[path.size() - 1] != '/') {
    log_error("Fail to remove directory, path must termniate with '/': %s", path.c_str());
    return false;
  }
  
#if (SKR_PLATFORM == SKR_PLATFORM_IOS) || (SKR_PLATFORM == SKR_PLATFORM_MAC)
  if (0 == nftw(path.c_str(), unlink_cb, 64, FTW_DEPTH | FTW_PHYS)) {
    return true;
  }
  else {
    return false;
  }
#elif (SKR_PLATFORM == SKR_PLATFORM_WIN32)
  std::string command = "cmd /c rd /s /q ";
  // Path may include space.
  command += "\"" + path + "\"";
  
  if (WinExec(command.c_str(), SW_HIDE) > 31)
    return true;
  else
    return false;
#else
  std::string command = "rm -r ";
  // Path may include space.
  command += "\"" + path + "\"";
  if (system(command.c_str()) >= 0)
    return true;
  else
    return false;
#endif
}

bool FileUtils::RemoveAllFilesInDirectory(const std::string& path) {
  if (path.size() > 0 && path[path.size() - 1] != '/') {
    log_error("Fail to remove directory, path must termniate with '/': %s", path.c_str());
    return false;
  }
  
  DIR *dir;
  struct dirent *ent;
  bool success = false;
  if ((dir = opendir (path.c_str())) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      if (ent->d_type == DT_DIR) {
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0)) { // ignore "." & ".." under unix
          std::string dir_path = path + ent->d_name + "/";
          success = RemoveDirectory(dir_path);
          SKR_ASSERT(success);
        }
      }
      else if (ent->d_type == DT_REG) {
        std::string file_path = path + ent->d_name;
        success = RemoveFile(file_path);
        SKR_ASSERT(success);
      }
      else {
        log_warning("FileUtils::RemoveAllFilesInDirectory will not delete such type of file, name is %s, type is %d",
                              ent->d_name, ent->d_type);
      }
    }
    closedir(dir);
    return true;
  } else {
    return false;
  }
  
  return false;
}

bool FileUtils::RemoveAllFilesInDirectory(const std::string& path, const std::vector<std::string>& exclude_sub_dirs) {
  if (path.size() > 0 && path[path.size() - 1] != '/') {
    log_error("Fail to remove directory, path must termniate with '/': %s", path.c_str());
    return false;
  }
  
  DIR *dir;
  struct dirent *ent;
  bool success = false;
  if ((dir = opendir (path.c_str())) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      // continue search if found excluded directories
      std::string ent_name = ent->d_name;
      if (std::find(exclude_sub_dirs.begin(), exclude_sub_dirs.end(), ent_name) != exclude_sub_dirs.end()) {
        continue;
      }
      
      if (ent->d_type == DT_DIR) {
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0)) { // ignore "." & ".." under unix
          // remove sub-dir
          std::string dir_path = path + ent->d_name + "/";
          success = RemoveDirectory(dir_path);
          SKR_ASSERT(success);
        }
      }
      else if (ent->d_type == DT_REG) {
        std::string file_path = path + ent->d_name;
        success = RemoveFile(file_path);
        SKR_ASSERT(success);
      }
      else {
        log_warning("FileUtils::RemoveAllFilesInDirectory will not delete such type of file, name is %s, type is %d",
                              ent->d_name, ent->d_type);
      }
    }
    closedir(dir);
    return true;
  } else {
    return false;
  }
  
  return false;
}

bool FileUtils::RemoveFile(const std::string &path) {
#if (SKR_PLATFORM == SKR_PLATFORM_WIN32)
  std::string command = "cmd /c del /q ";
  std::string win32path = path;
  int len = win32path.length();
  for (int i = 0; i < len; ++i)
  {
    if (win32path[i] == '/')
    {
      win32path[i] = '\\';
    }
  }
  command += win32path;
  
  if (WinExec(command.c_str(), SW_HIDE) > 31)
    return true;
  else
    return false;
#else
  if (remove(path.c_str())) {
    return false;
  } else {
    return true;
  }
#endif
}
