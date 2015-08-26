#ifndef TEST_ECHOSVR_SESSION_H
#define TEST_ECHOSVR_SESSION_H_

#include <string>

#include "event/event_handler.h"
#include "net/tcp/tcp_socket.h"
#include "sys/io_manager.h"

class echosvr_session : public eznetpp::event::connector_event_handler {
 public:
  echosvr_session(eznetpp::net::tcp::tcp_socket& sock);
  virtual ~echosvr_session();

 public:
  // override
  void on_recv(const std::string& msg, int len, int err_no);
  void on_send(unsigned int len, int err_no);
  void on_close(int err_no);

 private:
  eznetpp::net::tcp::tcp_socket& _socket;
  unsigned long _recv_cnt = 0;
  unsigned long _send_cnt = 0;
};

#endif  // TEST_ECHOSVR_SESSION_H_
