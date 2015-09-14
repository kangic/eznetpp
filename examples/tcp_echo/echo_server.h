#ifndef TEST_ECHO_SERVER_H_
#define TEST_ECHO_SERVER_H_

#include <string>

#include "eznetpp.h"
#include "net/tcp/tcp_acceptor.h"
#include "net/tcp/tcp_socket.h"
#include "sys/io_manager.h"

class echo_server : public eznetpp::event::tcp_acceptor_event_handler {
 public:
  echo_server(eznetpp::sys::io_manager* io_mgr);
  virtual ~echo_server();

 public:
  int open(int port, int backlog);

  // override
  void on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no);
  void on_close(int err_no);

 private:
  eznetpp::sys::io_manager* _io_mgr = nullptr;
  eznetpp::net::tcp::tcp_acceptor _acceptor;
  unsigned long _conns = 0;
};

#endif  // TEST_ECHO_SERVER_H_
