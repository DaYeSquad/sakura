#include <string>
#include <sstream>

#ifndef LESSCHATCORE_CORE_UTILS_ANDROID_STRING_UTILS_ANDROID_H_
#define LESSCHATCORE_CORE_UTILS_ANDROID_STRING_UTILS_ANDROID_H_

namespace std {

template<typename T>
std::string to_string(T value) {
  std::ostringstream os;
  os << value;
  return os.str();
}

} // namespace std

#endif //enddef LESSCHATCORE_CORE_UTILS_ANDROID_STRING_UTILS_ANDROID_H_
