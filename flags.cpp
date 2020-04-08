//
// Created by wuyongyu on 2020-04-08.
//

#include "flags.h"

template<class T>
T get_from_env(const char *name, const T &_default) {
  auto s = getenv(name);
  if (s == NULL) return _default;
  std::istringstream is(s);
  T env;
  if (is >> env) {
    is.peek();
    if (!is) {
      return env;
    }
  }
  std::cout << "Load" << name << "from env(" << s << ") failed, use default" << _default;
  return _default;
}
// 声明
template int get_from_env(const char *name, const int &_default);
template std::string get_from_env(const char *name, const std::string &_default);


// 重载std::string 类型
template<>
std::string get_from_env(const char *name, const std::string &_default) {
  auto s = getenv(name);
  if (s == NULL) return _default;
  return std::string(s);
}


