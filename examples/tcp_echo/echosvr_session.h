#ifndef TEST_ECHOSVR_SESSION_H
#define TEST_ECHOSVR_SESSION_H_

#include <string>

#include "event/event_handler.h"
#include "net/tcp/tcp_socket.h"
#include "sys/io_manager.h"

class echosvr_session : public eznetpp::event::tcp_socket_event_handler {
 public:
  echosvr_session(eznetpp::net::tcp::tcp_socket* sock);
  virtual ~echosvr_session();

 public:
  // override
  void on_recv(const std::string& msg, int len);
  void on_send(unsigned int len);
  void on_close(int err_no);

  // do not need 
  void on_connect(int err_no) {};

 private:
  eznetpp::net::tcp::tcp_socket* _socket;
  unsigned long _recv_cnt = 0;
  unsigned long _send_cnt = 0;
  long long _recv_bytes = 0;
  long long _send_bytes = 0;
};

#endif  // TEST_ECHOSVR_SESSION_H_
