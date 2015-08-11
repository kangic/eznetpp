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
  virtual ~socket(void);

 public:
  enum socket_domain { inet_v4 = PF_INET, inet_v6 = PF_INET6, unix = PF_LOCAL };
  enum socket_type { tcp = SOCK_STREAM, udp = SOCK_DGRAM };

  typedef struct _peer_addr {
    std::string ip = "";
    int port = -1;
  } peer_addr;

  // socket descriptor
  int descriptor(void) const { return _sd; }

  socket_domain domain(void);
  socket_type type(void);

  int set_nonblock(bool flag);
  int set_tcpnodelay(bool flag);
  int set_reuseaddr(bool flag);

  void register_handler(eznetpp::event::event_handler* handler) {
    _handler = handler;
  };

  /*
   * below functions are implemented by the inherited class.
   */
  virtual int bind_and_listen(int port, int max_accept_cnt = 5) = 0;
  virtual int accept(void) = 0;
  virtual int connect(const std::string& ip, int port) = 0;
  virtual int send(const char* msg, int len) = 0;
  virtual int recv(char* msg, int len) = 0; 
  virtual int close(void) = 0;

 protected:
  socket_domain _sock_domain = socket_domain::inet_v4;
  socket_type _sock_type = socket_type::tcp;
  int _sd = -1;
  peer_addr _peer;
  eznetpp::event::event_handler* _handler = nullptr;

 private:
  DISALLOW_COPY_AND_ASSIGN(socket);
};

}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_SOCKET_H_
