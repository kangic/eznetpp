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

#include "udp_socket.h"

#include <cstring>

namespace eznetpp {
namespace net {
namespace udp {

udp_socket::udp_socket(void)
{
  _sock_domain = socket_domain::inet_v4;
  _sock_type = socket_type::udp;

  // TODO : implement for PF_INET6
  _sd = ::socket(_sock_domain, _sock_type, 0);
  _desc = desc_udp_socket;

  set_nonblocking();
}

udp_socket::~udp_socket(void) {}

int udp_socket::open(int port)
{
  int ret = bind(port);

  if (ret != 0)
  {
    return ret;
  }

  set_reuseaddr();

  return 0;
}

eznetpp::event::io_event* udp_socket::_send(void)
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

    while (!_sendmsgs_vec.empty())
    {
      // step 2. pop a message from the send message's vector
      std::string msg = _sendmsgs_vec[0].first;
      eznetpp::net::peer_addr pi = _sendmsgs_vec[0].second;
      _sendmsgs_vec.erase(_sendmsgs_vec.begin());

      // step 3. send the message and check an error case
      struct sockaddr_in client_addr;
      bzero(&client_addr, sizeof(client_addr));
      client_addr.sin_addr.s_addr = inet_addr(pi.ip.c_str());
      client_addr.sin_port = htons((uint16_t)pi.port);
      int ret = (int)::sendto(_sd, msg.c_str(), msg.length(), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
      _last_errno = errno;

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

  if (send_bytes > 0)
  {
    return new eznetpp::event::io_event(eznetpp::event::event_type::udp_sendto, send_bytes);
  }
  else
  {
    return nullptr;
  }
}

eznetpp::event::io_event* udp_socket::_recv(int& ret)
{
  if (_sd == -1)
  {
    return nullptr;
  }

  std::string data(eznetpp::opt::max_transfer_bytes, '\0');
  struct sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  bzero(&client_addr, sizeof(client_addr));
  int len = (int)::recvfrom(_sd, &data[0], eznetpp::opt::max_transfer_bytes, 0
      , (struct sockaddr *)&client_addr, &client_addr_size);
  _last_errno = errno;
  ret = len;

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

  // create peer info
  set_peer_info(inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

  return new eznetpp::event::io_event(eznetpp::event::event_type::udp_recvfrom
      , len, std::move(data.assign(data, 0, len)), 0);

}

eznetpp::event::io_event* udp_socket::_close()
{
  int ret = ::close(_sd);
  _last_errno = errno;
  _sd = -1;

  return new eznetpp::event::io_event(eznetpp::event::event_type::close, ret);
}

void udp_socket::close()
{
  eznetpp::event::io_event* evt = _close();
  if (evt != nullptr)
  {
    delete evt;
  }

  _event_handler->on_close(0);
}

int udp_socket::bind(int port)
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

    close();
    return errno;
  }

  return 0;
}


}  // namespace udp
}  // namespace net
}  // namespace eznetpp
