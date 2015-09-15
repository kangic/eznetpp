#include "./udp_echo_server.h"

#include <cstdio>

udp_echo_server::udp_echo_server(eznetpp::sys::io_manager* io_mgr)
: _io_mgr(io_mgr) {
}

udp_echo_server::~udp_echo_server(void) {
}

int udp_echo_server::open(int port) {
  int ret = _socket.open(port);

  if (ret) {
    printf("%s\n", eznetpp::errcode::errno_to_str(ret).c_str());
    return -1;
  }
 
  _io_mgr->register_socket_event_handler(&_socket, this);

  return 0;
}

void udp_echo_server::on_recvfrom(const std::string& msg, int len
      , const std::string& peer_ip, int peer_port) {

  printf("received %d bytes from %s(%d)\n", len, peer_ip.c_str(), peer_port);

  _socket.send_bytes(msg, peer_ip, peer_port);
}

void udp_echo_server::on_sendto(unsigned int len) {
  printf("sent %d bytes\n", len);
}

void udp_echo_server::on_close(int err_no) {

}
