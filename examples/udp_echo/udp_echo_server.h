#ifndef TEST_UDP_ECHO_SERVER_H_
#define TEST_UDP_ECHO_SERVER_H_

#include <string>

#include "eznetpp.h"
#include "net/udp/udp_socket.h"
#include "sys/io_manager.h"

class udp_echo_server : public eznetpp::event::udp_socket_event_handler {
 public:
  udp_echo_server(eznetpp::sys::io_manager* io_mgr);
  virtual ~udp_echo_server();

 public:
  int open(int port);

  // override
  void on_recvfrom(const std::string& msg, int len
      , const std::string& peer_ip, int peer_port, int err_no);
  void on_sendto(unsigned int len, int err_no);
  void on_close(int err_no);

 private:
  eznetpp::sys::io_manager* _io_mgr = nullptr;
  eznetpp::net::udp::udp_socket _socket;
  unsigned long _conns = 0;
};

#endif  // TEST_UDP_ECHO_SERVER_H_
