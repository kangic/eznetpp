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

#include "net/udp/udp_socket.h"
#include "sys/io_manager.h"

class udp_echo_client : public eznetpp::event::udp_socket_event_handler {
 public:
  udp_echo_client(eznetpp::sys::io_manager* io_mgr) {
    _io_mgr = io_mgr;
  }
  virtual ~udp_echo_client() = default;

 public:
  void start() {
    _io_mgr->register_socket_event_handler(&_socket, this);

    _socket.send_bytes("echotest", "127.0.0.1", 56789);
  }

  // override
  void on_recvfrom(const std::string& msg, int len
      , const std::string& peer_ip, int peer_port) {
    printf("received %d bytes from %s(%d)\n", len, peer_ip.c_str(), peer_port);
    _socket.send_bytes(msg, peer_ip, peer_port);
  }
  void on_sendto(int len) {
    printf("sent %d bytes\n", len);
  }
  void on_close(int err_no) {
    printf("closed the socket(%d)\n", err_no);
  }

 private:
  eznetpp::sys::io_manager* _io_mgr = nullptr;
  eznetpp::net::udp::udp_socket _socket;
};

int main(void) {
  eznetpp::sys::io_manager io_mgr;
  io_mgr.init();

  udp_echo_client client(&io_mgr);
  client.start();
  
  io_mgr.loop();

  return 0;
}
