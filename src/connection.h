// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_CONNECTION_H_
#define INCLUDE_CONNECTION_H_

#include <eznetpp.h>

namespace eznetpp {
class connection {
 public:
  connection() {}
  virtual ~connection() {}

 public:
  void socket_id(int id) { _sock_id = id; }
  int socket_id() { return _sock_id; }

  // override by user
  virtual void on_recv(const std::string& msg, int len, int err_no) = 0;
  virtual void on_send(unsigned int len, int err_no) = 0;
  virtual void on_close(int err_no) = 0;

  // override by lib
  virtual int recv_from_socket(void) = 0;
  virtual int send_to_socket(const std::string& msg, int len) = 0;
  virtual int close_socket(void) = 0;

 protected:
  int _sock_id;
};
}  // namespace eznetpp

#endif  // INCLUDE_CONNECTION_H_
