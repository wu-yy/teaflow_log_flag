//
// Created by wuyongyu on 2020-04-08.
//

#pragma once
#include <iostream>
#include <map>
#include <tuple>
#include <sstream>

template<class T>
T get_from_env(const char *name, const T &_default);

template<>
std::string get_from_env(const char *name, const std::string &_default);

#define DECLARE_FLAG(type, name) \
extern "C" type name; \
extern "C" std::string doc_ ## name; \
extern "C" void set_ ## name (const type& t);

// #define TEA_JIT
#ifdef TEA_JIT
#define DEFINE_FLAG(type, name, default, doc) \
DECLARE_FLAG(type, name)
#define DEFINE_FLAG_WITH_SETTER(type, name, default, doc, setter) \
    DECLARE_FLAG(type, name)
#else

#define DEFINE_FLAG(type, name, default, doc) \
    DECLARE_FLAG(type, name); \
    type name; \
    std::string doc_  ## name = doc; \
    void set_ ## name (const type& value) { \
      name = value; \
    } \
    void init_ ## name (const type& value) { \
      name = value; \
      if (getenv(#name)) { } \
    } \
    int caller_ ## name = (init_ ## name(get_from_env<type>(#name, default)), 0);

#define DEFINE_FLAG_WITH_SETTER(type, name, default, doc) \
    DECLARE_FLAG(type, name); \
    type name; \
    std::string doc_ ## name = doc; \
    void setter_ ## name (type value); \
    void set_ ## name (const type& value) { \
        setter_ ## name (value); \
        name = value; \
    }; \
    void init_ ## name (const type& value) { \
        setter_ ## name (value); \
        name = value; \
    }; \
    int caller_ ## name = (init_ ## name (get_from_env<type>(#name, default)), 0);
#endif




//// 在.h 文件声明
//DECLARE_FLAG(int, log_v);
//// 在.cc 文件定义
//DEFINE_FLAG(int, log_v, 1, "");