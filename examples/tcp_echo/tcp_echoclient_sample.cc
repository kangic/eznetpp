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

    _socket.connect("127.0.0.1", 56789);
  };

  // override
  void on_connect(int err_no) {
    _socket.send_bytes("echotest");
  }
  void on_recv(const std::string& msg, int len) {
    printf("received %d bytes\n", len);
    _socket.send_bytes(msg);
  }
  void on_send(unsigned int len) {
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
