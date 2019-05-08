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

#include "sys/io_manager.h"

class tcp_echo_client : eznetpp::event::tcp_socket_event_handler {
 public:
  tcp_echo_client(eznetpp::sys::io_manager* io_mgr) {
    _io_mgr = io_mgr;
  }
  virtual ~tcp_echo_client() = default;

 public:
  void start() {
    _io_mgr->register_socket_event_handler(&_socket, this);

    if (0 == _socket.connect("127.0.0.1", 56789))
    {
      _socket.send_bytes("echotest");
    }
  };

  // override
  void on_recv(const std::string& msg, int len) {
    printf("received %d bytes\n", len);
    _socket.send_bytes(msg);
  }
  void on_send(int len) {
    printf("sent %d bytes\n", len);
  }
  void on_close(int err_no) {
    printf("closed the connection(%d)\n", err_no);
  }

 private:
  eznetpp::sys::io_manager* _io_mgr = nullptr;
  eznetpp::net::tcp::tcp_socket _socket;
};

int main(void) {
  eznetpp::sys::io_manager io_mgr;

  io_mgr.init();

  tcp_echo_client client(&io_mgr);
  client.start();

  io_mgr.loop();

  return 0;
}
