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

#include "tcp_socket.h"
#include "event/event.h"
#include "event/event_dispatcher.h"

#include <cstring>

namespace eznetpp {
namespace net {
namespace tcp {

tcp_socket::tcp_socket()
{
  _sock_domain = socket_domain::inet_v4;
  _sock_type = socket_type::tcp;

  // TODO : implement for PF_INET6
  _sd = ::socket(_sock_domain, _sock_type, 0);
  _desc = desc_tcp_socket;
}

tcp_socket::tcp_socket(int sd)
{
  _sock_domain = socket_domain::inet_v4;
  _sock_type = socket_type::tcp;
  _sd = sd;
  _desc = desc_tcp_socket;
}

tcp_socket::~tcp_socket() {}

int tcp_socket::connect(const std::string& ip, int port)
{
  if (_sd == -1)
  {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "socket() error(%d)", errno);
    return -1;
  }

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
  server_addr.sin_port = htons(port);

  // connect to server(on_connect event)
  int ret = ::connect(_sd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  if (ret == 0)
  {
    set_peer_info(ip, port);
  }

  return ret;
}

eznetpp::event::io_event* tcp_socket::_send()
{
  if (_sd == -1)
  {
    return nullptr;
  }

  int send_bytes = 0;
  std::lock_guard<std::mutex> lock(_sendmsgs_mtx);
  {
    // step 1. check the send message vector's size
    // if it isn't empty, start the loop statement to send the message.
    while (!_sendmsgs_vec.empty()) {
      // step 2. pop a message from the send message's vector
      std::string msg = _sendmsgs_vec[0].first;
      _sendmsgs_vec.erase(_sendmsgs_vec.begin());

      // step 3. send the message and check an error case
      int ret = (int)::send(_sd, msg.c_str(), msg.length(), 0);

      if (ret == -1)
      {
        // error check
        if (errno == EAGAIN)
        {
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
          continue;
        }

        eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
            , __FILE__, __FUNCTION__, __LINE__
            , "send() error(%d)", errno);
        close();
        return nullptr;
      }

      if (ret > 0)
      {
        send_bytes += ret;

      }
    }
  } // lock_guard

  return new eznetpp::event::io_event(eznetpp::event::event_type::tcp_send, send_bytes);
}

eznetpp::event::io_event* tcp_socket::_recv()
{
  printf("tcp_socket _recv()\n");
  if (_sd == -1)
  {
    return nullptr;
  }

  std::string data(eznetpp::opt::max_transfer_bytes, '\0');
  int len = (int)::recv(_sd, &data[0], eznetpp::opt::max_transfer_bytes, 0);
  _last_errno = errno;

  if (len == 0)
  {
    close();
    return nullptr;
  }
  else if (len == -1)
  {
    if (errno == EAGAIN || errno == EWOULDBLOCK)
    {
      return nullptr;
    }

    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
        , __FILE__, __FUNCTION__, __LINE__
        , "recv() error(%d)", errno);

    close();
    return nullptr;
  }

  return new eznetpp::event::io_event(
      eznetpp::event::event_type::tcp_recv, len, std::move(data.assign(data, 0, len)), 0);
}

eznetpp::event::io_event* tcp_socket::_close()
{
  int ret = ::close(_sd);
  _last_errno = errno;
  _sd = -1;

  return new eznetpp::event::io_event(eznetpp::event::event_type::close, ret);
}

void tcp_socket::close()
{
  eznetpp::event::io_event* evt = _close();
  if (evt != nullptr)
  {
    delete evt;
  }

  get_event_handler()->on_close(0);
}

}  // namespace tcp
}  // namespace net
}  // namespace eznetpp
