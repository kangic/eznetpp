// Copyright[2015] <kangic21@gmail.com>

#include <cstdio>
#include <cstdarg>

#include "logger.h"

namespace eznetpp {
namespace util {

logger* logger::_inst;
std::mutex logger::_log_mutex;

const char* log_level_str[] = {"FATAL", "ERROR", "WARN", "INFO", "DEBUG", };

logger::logger() {
}

logger::~logger() {
}

logger& logger::instance() {
  std::lock_guard<std::mutex> guard(logger::_log_mutex);

  if (_inst == nullptr)
    _inst = new logger();

  return *_inst;
}

void logger::set_enable_option(bool enable) {
  _log_enable = enable;
}

logger::cleanup::~cleanup() {
  std::lock_guard<std::mutex> guard(logger::_log_mutex);

  if (logger::_inst != nullptr) {
    delete logger::_inst;
    logger::_inst = nullptr;
  }
}

void logger::log(log_level level, const char* file, const char* func, int line
                 , const char* format, ...) {
  if (_log_enable)
  {
    std::lock_guard<std::mutex> guard(logger::_log_mutex);

    printf("[%s] ", log_level_str[level]);

    va_list arg;
    int count;

    va_start(arg, format);
    count = vprintf(format, arg);
    va_end(arg);

    printf("(%s::%s::%d)\n", file, func, line);
  }
}

}  // namespace util
}  // namespace eznetpp
