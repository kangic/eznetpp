// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_SOCKET_H_
#define INCLUDE_TCP_SOCKET_H_

#include "eznetpp.h"
#include "net/socket.h"
#include "event/event_handler.h"

namespace eznetpp {
namespace net {
namespace tcp {
class tcp_socket
: public eznetpp::net::socket {
 public:
  tcp_socket(void);
  virtual ~tcp_socket(void);

  int bind_and_listen(int port, int max_accept_cnt);
  int accept(void);
  int connect(const std::string& ip, int port);
  int send(const char* msg, int len);
  int recv(char* msg, int len); 
  int close(void);



};

}  // namespace tcp
}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_TCP_SOCKET_H_
