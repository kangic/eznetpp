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

#ifndef INCLUDE_IO_EVENT_H_
#define INCLUDE_IO_EVENT_H_

#include "eznetpp.h"

namespace eznetpp {
namespace net { class if_socket; }
namespace event {

  enum event_type {
    close = 0,
    tcp_accept,
    tcp_connect,
    tcp_recv,
    tcp_send,
    udp_recvfrom,
    udp_sendto,
  };

class io_event {
 public:
  io_event(void) = delete;

  io_event(event_type type) {
   _type = type;
  }

  io_event(event_type type, int result) {
   _type = type;
   _result = result;
  }

  io_event(event_type type, int result, const std::string& data, int opt_data) {
   _type = type;
   _result = result;
   _data = data;
   _opt_data = opt_data;
  }

  virtual ~io_event(void) {};

  event_type type(void) { return _type; }
  int result(void) { return _result; }
  const std::string& data(void) { return _data; }
  int opt_data(void) { return _opt_data; }

 private:
  event_type _type = event_type::close;
  int _result = 0;
  std::string _data = "";
  int _opt_data = 0;
};

}  // namespace event
}  // namespace eznetpp

#endif  // INCLUDE_IO_EVENT_H_
