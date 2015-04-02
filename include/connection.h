// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_CONNECTION_H_
#define INCLUDE_CONNECTION_H_

#include <string>

#include "./macros.h"

namespace eznetpp {
class connection {
 public:
  connection() {};
  virtual ~connection() {};

 public:
  void socket(int id) { sock_id = id; }
  int socket() { return sock_id; }

  virtual int read(std::string data, int* received_len) = 0;
  virtual int write(const std::string& data, int len) = 0;

 protected:
  int sock_id;
};
}  // namespace eznetpp

#endif  // INCLUDE_CONNECTION_H_
