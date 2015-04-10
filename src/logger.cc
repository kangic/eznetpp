// Copyright[2015] <kangic21@gmail.com>

#include <cstdio>
#include <cstdarg>

#include "../include/logger.h"

namespace eznetpp {

logger* logger::_inst;
#if __cplusplus > 199711L
std::mutex logger::_log_mutex;
#endif

const char* log_level_str[] = {"FATAL", "ERROR", "WARN", "INFO", "DEBUG", };

logger::logger() {
}

logger::~logger() {
}

logger& logger::instance() {
  // TODO(kangic) lock : pthread_mutex
#if __cplusplus > 199711L
  std::lock_guard<std::mutex> guard(logger::_log_mutex);
#endif

  if (_inst == nullptr)
    _inst = new logger();

  return *_inst;
}

logger::cleanup::~cleanup() {
  // TODO(kangic) lock : pthread_mutex
#if __cplusplus > 199711L
  std::lock_guard<std::mutex> guard(logger::_log_mutex);
#endif

  if (logger::_inst != nullptr) {
    delete logger::_inst;
    logger::_inst = nullptr;
  }
}

void logger::log(log_level level, const char* format, ...) {
  // TODO(kangic) lock : pthread_mutex
#if __cplusplus > 199711L
  std::lock_guard<std::mutex> guard(logger::_log_mutex);
#endif

  printf("[%s] ", log_level_str[level]);

  va_list arg;
  int count;

  va_start(arg, format);
  count = vprintf(format, arg);
  va_end(arg);
}
}  // namespace eznetpp
