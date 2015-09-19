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
#include "event/event.h"
#include "event/event_dispatcher.h"

#include <cstring>

namespace eznetpp {
namespace net {
namespace udp {

udp_socket::udp_socket(void) {
  _sock_domain = socket_domain::inet_v4;
  _sock_type = socket_type::udp;

  // TODO : implement for PF_INET6
  _sd = ::socket(_sock_domain, _sock_type, 0);

  set_nonblocking();
}

udp_socket::~udp_socket(void) {
}

int udp_socket::open(int port) {
  int ret = bind(port);

  if (ret != 0)
    return ret;

  set_reuseaddr();

  return 0;
}

void udp_socket::send(void) {
  if (_sd == -1)
    return;

  {
    std::lock_guard<std::mutex> lock(_sendmsgs_mtx);
    // step 1. check the send message vector's size
    // if it isn't empty, start the loop statement to send the message. 
    
    while (!_sendmsgs_vec.empty()) {
      // step 2. pop a message from the send message's vector
      std::string msg = _sendmsgs_vec[0].first;
      eznetpp::net::peer_addr pi = _sendmsgs_vec[0].second;
      _sendmsgs_vec.erase(_sendmsgs_vec.begin());

      // step 3. send the message and check an error case
      struct sockaddr_in client_addr;
      bzero(&client_addr, sizeof(client_addr));
      client_addr.sin_addr.s_addr = inet_addr(pi.ip.c_str());
      client_addr.sin_port = htons(pi.port);
      int ret = ::sendto(_sd, msg.c_str(), msg.length(), 0
          , (struct sockaddr *)&client_addr, sizeof(client_addr));
      _last_errno = errno;
      if (ret == -1) {
        // error check
        if (errno == EAGAIN) {
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
          continue;
        }

        break;
      }
      if (ret > 0) {
        eznetpp::event::event_dispatcher::instance().push_event(
            new eznetpp::event::io_event(eznetpp::event::event_type::udp_sendto
              , ret, this));
      }
    }
  } // lock_guard
  
  if (set_epollout_flag(false) == -1)
      eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
          , __FILE__, __FUNCTION__, __LINE__
          , "epoll_ctl() error(%d)", errno);
}

void udp_socket::recv(void) {
  if (_sd == -1)
    return;

  bool read_again = 1;
  while (read_again) {
    std::string data(eznetpp::opt::max_transfer_bytes, '\0');
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    bzero(&client_addr, sizeof(client_addr));
    int len = ::recvfrom(_sd, &data[0], eznetpp::opt::max_transfer_bytes, 0
        , (struct sockaddr *)&client_addr, &client_addr_size);
    _last_errno = errno;
    read_again = 0;
    if (len == 0) {
      close();
      break;
    } else if (len == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        break;

      eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
          , __FILE__, __FUNCTION__, __LINE__
          , "recv() error(%d)", errno);
      close();
      break;
    }
    if (len == eznetpp::opt::max_transfer_bytes) {
      read_again = 1;
    }

    // create peer info
    set_peer_info(inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    eznetpp::event::event_dispatcher::instance().push_event(
        new eznetpp::event::io_event(eznetpp::event::event_type::udp_recvfrom
          , len, std::move(data.assign(data, 0, len)), 0, this));
  }
}

void udp_socket::close(void) {
  int ret = ::close(_sd);
  _sd = -1;

  eznetpp::event::event_dispatcher::instance().push_event(
      new eznetpp::event::io_event(eznetpp::event::event_type::close, ret, this));
}

int udp_socket::bind(int port) {
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  server_addr.sin_port = htons(port);

  int ret = ::bind(_sd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  if (ret != 0) {
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
