// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_SOCKET_H_
#define INCLUDE_SOCKET_H_

#include "eznetpp.h"
#include "event/event_handler.h"

/*
 * Socket Interface Class
 */
namespace eznetpp {
namespace net {
class socket {
 public:
  socket(void);
  socket(int sd);
  virtual ~socket(void);

 public:
  enum socket_domain { inet = 0, unix = 1 };
  enum socket_type { stream = 0, dgram = 1 };

  // socket descriptor
  int descriptor(void) const { return _sd; }

  socket_domain domain(void);
  socket_type type(void);

  int set_nonblock(bool flag);
  int set_tcpnodelay(bool flag);
  int set_reuseaddr(bool flag);

  /*
   * below functions are implemented by the inherited class.
   */
  virtual int initialize() = 0;
  virtual int accept(eznetpp::event::event_handler* handler) = 0;
  virtual int connect(eznetpp::event::event_handler* handler) = 0;
  virtual int send(eznetpp::event::event_handler* handler) = 0;
  virtual int recv(eznetpp::event::event_handler* handler) = 0; 
  virtual int close(eznetpp::event::event_handler* handler) = 0;

 protected:
  socket_domain _sock_domain = socket_domain::inet;
  socket_type _sock_type = socket_type::stream;
  int _sd = -1;
};

}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_SOCKET_H_
