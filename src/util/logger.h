/* Copyright (c) 2015 In Cheol, Kang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef INCLUDE_LOGGER_H_
#define INCLUDE_LOGGER_H_

#include "eznetpp.h"

namespace eznetpp {
namespace util {
class logger
{
 public:
  // log level
  typedef enum _log_level
  {
    fatal = 0,
    error,
    warn,
    info,
    debug,

    max_depth
  } log_level;

  static logger& instance();

  void set_enable_option(bool enable);

  void log(log_level level, const char* file, const char* func, int line, const char* format, ...);

 protected:
  friend class cleanup;
  class cleanup
  {
   public:
    ~cleanup();
  };

  static logger* _inst;

 private:
  logger(void);
  virtual ~logger(void);

  static std::mutex _log_mutex;
  bool _log_enable = false;
};
}  // namespace util
}  // namespace eznetpp

#endif  // INCLUDE_LOGGER_H_
