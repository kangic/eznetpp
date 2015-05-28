// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_LOGGER_H_
#define INCLUDE_LOGGER_H_

#include <mutex>

#include "./macros.h"

namespace eznetpp {
class logger {
 public:
  // log level
  typedef enum _log_level {
    fatal = 0,
    error,
    warn,
    info,
    debug,

    max_depth
  } log_level;

  static logger& instance();

  void log(log_level level, const char* file, const char* func, int line
           , const char* format, ...);

 protected:
  friend class cleanup;
  class cleanup {
   public:
    ~cleanup();
  };

  static logger* _inst;

 private:
  logger(void);
  virtual ~logger(void);

  static std::mutex _log_mutex;

  DISALLOW_COPY_AND_ASSIGN(logger);
};
}  // namespace eznetpp

#endif  // INCLUDE_LOGGER_H_
