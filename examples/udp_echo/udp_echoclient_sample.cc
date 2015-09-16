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
  void on_sendto(unsigned int len) {
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
