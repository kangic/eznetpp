// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_CONNECTION_H_
#define INCLUDE_CONNECTION_H_

#include <string>
#include <mutex>

#include "./macros.h"

namespace eznetpp {
class connection {
 public:
  connection() {}
  virtual ~connection() {}

 public:
  void socket_id(int id) { _sock_id = id; }
  int socket_id() { return _sock_id; }

  // override by user
  virtual void on_read(const std::string& msg, int len, int err_no) = 0;
  virtual void on_write(unsigned int len, int err_no) = 0;
  virtual void on_close(int err_no) = 0;

  // override by lib
  virtual int read_from_socket(void) = 0;
  virtual int write_to_socket(const std::string& msg, int len) = 0;
  virtual int close_socket(void) = 0;

 protected:
  int _sock_id;
  std::mutex _sock_mutex;
};
}  // namespace eznetpp

#endif  // INCLUDE_CONNECTION_H_
