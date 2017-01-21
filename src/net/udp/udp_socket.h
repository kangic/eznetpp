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

#ifndef INCLUDE_UDP_SOCKET_H_
#define INCLUDE_UDP_SOCKET_H_

#include "eznetpp.h"
#include "net/if_socket.h"

namespace eznetpp {
namespace net {
namespace udp {
class udp_socket : public eznetpp::net::if_socket
{
 public:
  udp_socket(void);
  virtual ~udp_socket(void);

  int open(int port);
  void close(void);

 protected:
  eznetpp::event::io_event* _recv(int& ret);
  eznetpp::event::io_event* _send();
  eznetpp::event::io_event* _close();

 private:
  int bind(int port);
};

}  // namespace udp
}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_UDP_SOCKET_H_
