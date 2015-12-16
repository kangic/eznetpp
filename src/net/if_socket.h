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

#ifndef INCLUDE_SOCKET_H_
#define INCLUDE_SOCKET_H_

#include "eznetpp.h"

/*
 * Socket Interface Class
 */
namespace eznetpp {
namespace net {
class if_socket {
 public:
  if_socket(void);
  virtual ~if_socket(void);

  enum socket_domain { inet_v4 = PF_INET, inet_v6 = PF_INET6, unix = PF_LOCAL };
  enum socket_type { tcp = SOCK_STREAM, udp = SOCK_DGRAM };

  // socket descriptor
  int descriptor(void) const { return _sd; }
  void descriptor(int sd) { _sd = sd; }

  socket_domain domain(void);
  socket_type type(void);

  void set_peer_info(const std::string& ip, int port);
  struct _peer_addr& peer() { return _peer; }

  int set_nonblocking(void);
  int set_tcpnodelay(void);
  int set_reuseaddr(void);

  int set_epollout_flag(bool flag);

  int last_error(void) { return _last_errno; }

  // for user
  int send_bytes(const std::string& data, const std::string& ip = "", int port = 0);
  virtual void close(void) = 0;

  // Below functions will implemented by each inherited class
  // (ex : acceptor, connector)
  virtual void recv(void) = 0;
  virtual void send(void) = 0;

 protected:
  socket_domain _sock_domain = socket_domain::inet_v4;
  socket_type _sock_type = socket_type::tcp;
  int _sd = -1;
  int _last_errno = 0;

  peer_addr _peer;

  typedef std::pair<std::string, peer_addr> msg_pair;
  std::vector<msg_pair> _sendmsgs_vec;
  std::mutex _sendmsgs_mtx;
};

}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_SOCKET_H_
