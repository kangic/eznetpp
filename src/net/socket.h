// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_SOCKET_H_
#define INCLUDE_SOCKET_H_

#include "eznetpp.h"
#include "event/event_handler.h"

namespace eznetpp {
namespace net {
class socket {
 public:
  socket();
  virtual ~socket();

 public:
  enum socket_type { raw = 0, acceptor = 1 };

  void fd(int fd) { _sock_fd = fd; }
  int fd(void) const { return _sock_fd; }

  socket_type type(void);

  int set_nonblock(bool flag);
  int set_tcpnodelay(bool flag);
  int set_reuseaddr(bool flag);

  virtual int initialize() = 0;
  virtual int accept(eznetpp::event::event_handler* handler) = 0;
  virtual int send(eznetpp::event::event_handler* handler) = 0;
  virtual int recv(eznetpp::event::event_handler* handler) = 0; 
  virtual int close(eznetpp::event::event_handler* handler) = 0;

 protected:
  socket_type _sock_type = socket_type::raw;
  int _sock_fd = -1;
};

}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_SOCKET_H_
