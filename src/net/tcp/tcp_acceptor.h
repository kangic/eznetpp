// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_ACCEPTOR_H_
#define INCLUDE_TCP_ACCEPTOR_H_

#include "eznetpp.h"
#include "net/tcp/tcp_socket.h"

/*
 * Accept operation is handled by tcp_socket class.
 */
namespace eznetpp {
namespace net {
namespace tcp {
class tcp_acceptor : public tcp_socket {
 public:
  tcp_acceptor(void);
  virtual ~tcp_acceptor(void);

  int open(int port, int backlog = 5);

  void recv(void);
  void close(void);

 private:
  int bind_and_listen(int port, int backlog);
  int accept(void);
};

}  // namespace tcp
}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_TCP_ACCEPTOR_H_