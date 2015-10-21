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

#include "eznetpp.h"
#include "net/tcp/tcp_acceptor.h"
#include "net/tcp/tcp_socket.h"
#include "sys/io_manager.h"

eznetpp::sys::io_manager g_io_mgr(4);

void sig_handler(int signum) {
  g_io_mgr.stop();
}

class tcp_echosvr_session : public eznetpp::event::tcp_socket_event_handler {
 public:
  tcp_echosvr_session(eznetpp::net::tcp::tcp_socket* sock) {
    _socket = sock;
  }
  virtual ~tcp_echosvr_session() = default;

 public:
  // override
  void on_recv(const std::string& msg, int len) {
    printf("received %d bytes\n", len);
    _socket->send_bytes(msg);
  }
  void on_send(unsigned int len) {
    printf("sent %d bytes\n", len);
  }
  void on_close(int err_no) {
    printf("closed the session(%d)\n", err_no);
  }

  // do not need 
  void on_connect(int err_no) {};

 private:
  eznetpp::net::tcp::tcp_socket* _socket;
};

class tcp_echo_server : public eznetpp::event::tcp_acceptor_event_handler {
 public:
  tcp_echo_server(eznetpp::sys::io_manager* io_mgr) {
    _io_mgr = io_mgr;
  }
  virtual ~tcp_echo_server() = default;

  int open(int port, int backlog) {
    int ret = _acceptor.open(port, backlog);

    if (ret) {
      printf("%s\n", eznetpp::errcode::errno_to_str(ret).c_str());
      return -1;
    }

    _io_mgr->register_socket_event_handler(&_acceptor, this);

    return 0;
  }

  // override
  void on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no) {
    _io_mgr->register_socket_event_handler(sock, new tcp_echosvr_session(sock));
  }
  void on_close(int err_no) {
    printf("closed the acceptor(%d)\n", err_no);
  }

 private:
  eznetpp::sys::io_manager* _io_mgr = nullptr;
  eznetpp::net::tcp::tcp_acceptor _acceptor;
};

int main(void) {
  signal(SIGINT, sig_handler);

  g_io_mgr.init();

  tcp_echo_server server(&g_io_mgr);
  server.open(56789, 5);

  g_io_mgr.loop();

  return 0;
}
