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

#include "tcp_acceptor.h"

#include "event/event.h"
#include "event/event_dispatcher.h"

namespace eznetpp {
namespace net {
namespace tcp {

tcp_acceptor::tcp_acceptor()
{
  _desc = desc_tcp_acceptor;
}

tcp_acceptor::~tcp_acceptor()
{
}

int tcp_acceptor::open(int port, int backlog)
{
  int ret = bind_and_listen(port, backlog);

  if (ret != 0)
    return ret;

  set_nonblocking();
  set_reuseaddr();

  return 0;
}

eznetpp::event::io_event* tcp_acceptor::_recv(int &ret)
{
  int sock_fd;
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  sock_fd = ::accept(_sd, (struct sockaddr *)&client_addr, &client_addr_len);
  _last_errno = errno;

  ret = sock_fd;

  if (sock_fd == -1)
  {
    if (errno == EAGAIN || errno == EWOULDBLOCK)
    {
      // It is not an error case.
      return nullptr;
    }
  }

  return new eznetpp::event::io_event(eznetpp::event::event_type::tcp_accept
            , sock_fd
            , inet_ntoa(client_addr.sin_addr)
            , ntohs(client_addr.sin_port));
}

int tcp_acceptor::bind_and_listen(int port, int backlog)
{
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(port);

  int ret = ::bind(_sd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  if (ret != 0)
  {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
        , __FILE__, __FUNCTION__, __LINE__
        , "::bind() error(%d)", errno);

    ::close(_sd);
    _sd = -1;
    return errno;
  }

  ret = ::listen(_sd, backlog);

  if (ret != 0)
  {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
        , __FILE__, __FUNCTION__, __LINE__
        , "::listen() error(%d)", errno);

    ::close(_sd);
    _sd = -1;
    return errno;
  }

  return 0;
}


}  // namespace tcp
}  // namespace net
}  // namespace eznetpp
