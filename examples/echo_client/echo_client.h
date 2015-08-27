#ifndef TEST_ECHO_CLIENT_H_
#define TEST_ECHO_CLIENT_H_

#include <string>

#include "net/tcp/tcp_socket.h"
#include "sys/io_manager.h"

class echo_client : eznetpp::event::socket_event_handler {
 public:
  echo_client(eznetpp::sys::io_manager* io_mgr);
  virtual ~echo_client();

 public:
  void start();

  // override
  void on_connect(int err_no);
  void on_recv(const std::string& msg, int len, int err_no);
  void on_send(unsigned int len, int err_no);
  void on_close(int err_no);

 private:
  eznetpp::sys::io_manager* _io_mgr = nullptr;
  eznetpp::net::tcp::tcp_socket _socket;
};

#endif  // TEST_ECHO_CLIENT_H_
