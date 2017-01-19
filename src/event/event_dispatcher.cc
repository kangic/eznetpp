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

#include "event_dispatcher.h"
/*
#include "net/if_socket.h"
#include "net/tcp/tcp_socket.h"
*/

namespace eznetpp {
namespace event {

event_dispatcher::event_dispatcher(void)
{
}

event_dispatcher::~event_dispatcher(void)
{
}

void event_dispatcher::dispatch_event(const io_event& evt, const if_event_handler* handler)
{

}
/*
void event_dispatcher::dispatch_event(const io_event& evt, const event_handler& handler)
{
  auto sock = evt.publisher();

  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
      , __FILE__, __FUNCTION__, __LINE__
      , "io_event : %p(type : %d), socket : %p, handler : %p"
      , evt, evt.type(), &sock, handler);

  switch (evt.type())
  {
    case event::event_type::close:
    {
      // Delete the socket descriptor from epoll descriptor automatically
      // when the socket is closed.
      // Call the on_close event in clear_resources() function.
      handler.on_close(0);

      if (sock != nullptr) {
        delete sock;
        sock = nullptr;
      }

      break;
    }
    case event::event_type::tcp_accept:
    {
      int sock_fd = evt.result();

      auto tcp_sock = new eznetpp::net::tcp::tcp_socket(sock_fd);
      tcp_sock->set_peer_info(std::move(evt.data()), evt.opt_data());
      tcp_sock->set_nonblocking();
      tcp_sock->set_tcpnodelay();
      handler.on_accept(tcp_sock, 0);

      break;
    }
    case event::event_type::tcp_connect:
    {
      sock->set_peer_info(evt.data().c_str(), evt.opt_data());
      handler.on_connect(sock->last_error());

      break;
    }
    case event::event_type::tcp_recv:
    {
      handler.on_recv(evt.data(), evt.result());

      break;
    }
    case event::event_type::tcp_send:
    {
      handler.on_send(evt.result());

      break;
    }
    case event::event_type::udp_recvfrom:
    {
      handler.on_recvfrom(evt.data(), evt.result(), sock->peer().ip, sock->peer().port);

      break;
    }
    case event::event_type::udp_sendto:
    {
      handler.on_sendto(evt.result());

      break;
    }
  }
}
*/

}  // namespace event
}  // namespace eznetpp
