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

#ifndef INCLUDE_IF_EVENT_HANDLER_H_
#define INCLUDE_IF_EVENT_HANDLER_H_

#include "eznetpp.h"

namespace eznetpp {
namespace net { namespace tcp { class tcp_socket; } }
namespace event {

class if_event_handler
{
 public:
  if_event_handler(void) = default;
  virtual ~if_event_handler(void) = default;

  enum event_handler_type
  {
    tcp_socket = 0,
    tcp_acceptor,
    udp_socket,
  };
  event_handler_type type() { return _handler_type; };

  virtual void on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no) = 0;
  virtual void on_connect(int err_no) = 0;
  virtual void on_recv(const std::string& msg, int len) = 0;
  virtual void on_send(unsigned int len) = 0;
  virtual void on_recvfrom(const std::string& msg, int len
      , const std::string& peer_ip, int peer_port) = 0;
  virtual void on_sendto(unsigned int len) = 0;
  virtual void on_close(int err_no) = 0;

 protected:
  event_handler_type _handler_type;
};


}  // namespace event
}  // namespace eznetpp

#endif  // INCLUDE_IF_EVENT_HANDLER_H_
