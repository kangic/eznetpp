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

    va_start(arg, format);
    vprintf(format, arg);
    va_end(arg);

    printf("(%s::%s::%d)\n", file, func, line);
  }
}

}  // namespace util
}  // namespace eznetpp
