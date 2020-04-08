//
// Created by wuyongyu on 2020-04-08.
//
#pragma once
#include <string>
#include <sstream>
#include <functional>
#include <iostream>
#include "flags.h"
using string = std::string;

namespace teaflow {
DECLARE_FLAG(int, log_silent);
DECLARE_FLAG(int, log_sync);
DECLARE_FLAG(int, log_v);
DECLARE_FLAG(std::string, log_vprefix);
void print_trace();
extern "C" uint32_t get_tid();
extern "C" bool g_supports_color;
extern "C" void print_prefix(std::ostream* out);

const char green[] = "\033[38;5;2m";
const char red[] = "\033[38;5;1m";
const char yellow[] = "\033[38;5;3m";

static void get_color(char level, int verbose, const char*& color_begin, const char*& color_end) {
  if (level == 'i') {
    if (verbose == 0) {
      color_begin = "\033[38;5;2m";
    } else if (verbose < 10) {
      color_begin = "\033[38;5;250m";
    } else if (verbose < 100) {
      color_begin = "\033[38;5;244m";
    } else if (verbose < 1000) {
      color_begin = "\033[38;5;238m";
    } else {
      color_begin = "\033[38;5;232m";
    }
  } else if (level == 'w') {
    color_begin = yellow;
  } else if (level == 'e') {
    color_begin = red;
  } else { // level == 'f'  // fatal
    color_begin = red;
  }
  color_end = "\033[m";
}

extern "C" void send_log(std::ostringstream&& out);
extern "C" void flush_log();
extern "C" void log_capture_start();
extern "C" void log_capture_stop();
extern std::vector<std::map<string,string>> log_capture_read();

struct Log {
  std::ostringstream out;
  const char* color_end;

  Log(const char* const fileline, char level, int verbose) {
    const char* color_begin;
    get_color(level, verbose, color_begin, color_end);
    if (g_supports_color) out << color_begin;
    out << '[' << level << ' ';
    print_prefix(&out);
    if (verbose) out << 'v' << verbose << ' ';
    out << fileline << ']';
  }

  void end() {
    if (g_supports_color) out << color_end;
    out << '\n';
    send_log(move(out));
  }
  void flush() { flush_log(); }

  template<class T>
  Log& operator<<(const T& a) { out << ' ' << a; return *this; }
  template<class T>
  Log& operator>>(const T& a) { out << a; return *this; }
};

// 打印日志
struct LogVoidify {
  void operator&&(Log& log) { log.end(); }
};

// 程序Fatal 抛出错误
struct LogFatalVoidify {
  void operator&&(Log& log) {
    log.flush();
    if (g_supports_color) log.out << log.color_end;
    throw std::runtime_error(log.out.str());
  }
};

constexpr int32_t basename_index(const char * const path, const int32_t index = 0, const int32_t slash_index = -1) {
  return path[index]
         ? ((path[index] == '/' || path[index] == '\\')
            ? basename_index (path, index + 1, index)
            : basename_index (path, index + 1, slash_index)
         )
         : (slash_index + 1);
}

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)

#define __FILELINE__ \
    (&((__FILE__ ":" STRINGIZE(__LINE__))[teaflow::basename_index(__FILE__)]))


#define _LOGi(v) LogVoidify() && Log(__FILELINE__, 'i', v)
#define _LOGw(v) LogVoidify() && Log(__FILELINE__, 'w', v)
#define _LOGe(v) LogVoidify() && Log(__FILELINE__, 'e', v)
#define _LOGf(v) LogFatalVoidify() && Log(__FILELINE__, 'f', v)
#define LOGi _LOGi(0)
#define LOGw _LOGw(0)
#define LOGe _LOGe(0)
#define LOGf _LOGf(0)

#define _LOG(level, v) _LOG ## level(v)
#define LOG(level) _LOG(level, 0)

#define _LOG_IF(level, cond, v) \
    !(cond) ? (void) 0 : _LOG(level, v)
#define LOG_IF(level, cond) _LOG_IF(level, cond, 0)

bool check_vlog(const char* fileline, int verbose);

#define V_ON(v) PREDICT_BRANCH_NOT_TAKEN(log_vprefix.size() ? \
        check_vlog(__FILELINE__, v) : \
        (v) <= log_v)

#define LOGV(v) \
    _LOG_IF(i, log_vprefix.size() ? \
        teaflow::check_vlog(__FILELINE__, v) : \
        (v) <= log_v, v)


// __builtin_expect(!!(x), 0) //x很可能为假
// __builtin_expect(!!(x), 1) //x很可能为真
#define PREDICT_BRANCH_NOT_TAKEN(x) (__builtin_expect(x, 0))

// CHECK 函数
#define CHECK(cond) \
    LOG_IF(f, PREDICT_BRANCH_NOT_TAKEN(!(cond))) \
        << "Check failed: " #cond " "

#define LOGv LOGV(1)
#define LOGvv LOGV(10)
#define LOGvvv LOGV(100)
#define LOGvvvv LOGV(1000)
#define CHECKop(a, op, b) LOG_IF(f, !((a) op (b))) \
    << "Check failed" \
    << #a "(" >> a >> ") " #op " " #b"(" >> b >> ")"

#define ASSERT(s) CHECK(s) << "Something wrong... Could you please report this issue?\n"
#define ASSERTop(a, op, b) CHECKop(a, op, b) << "Something wrong ... Could you please report this issue?\n"

#define LOGg LOGv >> teaflow::green
#define LOGr LOGv >> teaflow::red
#define LOGy LOGv >> teaflow::yellow
#define LOGgg LOGvv >> teaflow::green
#define LOGrr LOGvv >> teaflow::red
#define LOGyy LOGvv >> teaflow::yellow
#define LOGggg LOGvvv >> teaflow::green
#define LOGrrr LOGvvv >> teaflow::red
#define LOGyyy LOGvvv >> teaflow::yellow
#define LOGgggg LOGvvvv >> teaflow::green
#define LOGrrrr LOGvvvv >> teaflow::red
#define LOGyyyy LOGvvvv >> teaflow::yellow

}   // end namespace teaflow
