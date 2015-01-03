#ifndef _EZNETPP_SRC_SERVER_SOCKET_H_
#define _EZNETPP_SRC_SERVER_SOCKET_H_

#include "./macros.h"

namespace eznetpp {
class server_socket {
 public:
  server_socket() {}
  virtual ~server_socket() {}

 public:
  void make_socket() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(server_socket);
};
}  // namespace eznetpp

#endif  // _EZNETPP_SRC_SERVER_SOCKET_H_
