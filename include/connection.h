// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_CONNECTION_H_
#define INCLUDE_CONNECTION_H_

#include <string>

#include "./macros.h"

namespace eznetpp {
class connection {
 public:
  connection() {}
  virtual ~connection() {}

 public:
  void socket(int id) { sock_id = id; }
  int socket() { return sock_id; }

  // override by user
  virtual void on_read(const std::string& msg, int len, int err_no) = 0;
  virtual void on_write(unsigned int len, int err_no) = 0;
  virtual void on_close(int err_no) = 0;

  // override by lib
  virtual int read(std::string msg, int* received_len) = 0;
  virtual int write(const std::string& msg, int len) = 0;
  virtual int close() = 0;

  int sock_id;
};
}  // namespace eznetpp

#endif  // INCLUDE_CONNECTION_H_
