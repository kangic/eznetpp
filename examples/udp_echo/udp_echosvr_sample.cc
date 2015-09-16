#include "net/udp/udp_socket.h"
#include "sys/io_manager.h"

class udp_echo_server : public eznetpp::event::udp_socket_event_handler {
 public:
  udp_echo_server(eznetpp::sys::io_manager* io_mgr) {
    _io_mgr = io_mgr;
  }
  virtual ~udp_echo_server() = default;

 public:
  int open(int port) {
    int ret = _socket.open(port);

    if (ret) {
      printf("%s\n", eznetpp::errcode::errno_to_str(ret).c_str());
      return -1;
    }

    _io_mgr->register_socket_event_handler(&_socket, this);

    return 0;
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

  udp_echo_server server(&io_mgr);
  server.open(56789);
  
  io_mgr.loop();

  return 0;
}
