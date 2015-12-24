#include <string>
#include <sstream>

#ifndef SAKURA_ANDROID_STRING_UTILS_ANDROID_H_
#define SAKURA_ANDROID_STRING_UTILS_ANDROID_H_

namespace std {

template<typename T>
std::string to_string(T value) {
  std::ostringstream os;
  os << value;
  return os.str();
}

} // namespace std

#endif //enddef SAKURA_ANDROID_STRING_UTILS_ANDROID_H_
