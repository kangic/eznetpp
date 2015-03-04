// Copyright[2015] <kangic21@gmail.com>

#include <cstdio>
#include <cstdarg>

#include "../include/logger.h"

namespace eznetpp {

logger* logger::inst;
std::mutex logger::log_mutex;

const char* log_level_str[] = {"FATAL", "ERROR", "WARN", "INFO", "DEBUG", };

logger::logger() {
}

logger::~logger() {
}

logger& logger::instance() {
  std::lock_guard<std::mutex> guard(log_mutex);
  if (inst == nullptr)
    inst = new logger();

  return *inst;
}

logger::cleanup::~cleanup() {
  std::lock_guard<std::mutex> guard(logger::log_mutex);
  if (logger::inst != nullptr) {
    delete logger::inst;
    logger::inst = nullptr;
  }
}

void logger::log(log_level level, const char* format, ...) {
  std::lock_guard<std::mutex> guard(log_mutex);
  printf("[%s] ", log_level_str[level]);

  va_list arg;
  int count;

  va_start(arg, format);
  count = vprintf(format, arg);
  va_end(arg);
}


}  // namespace eznetpp
