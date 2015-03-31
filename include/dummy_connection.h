// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_DUMMY_CONNECTION_H_
#define INCLUDE_DUMMY_CONNECTION_H_

#include <string>

#include "./macros.h"

namespace eznetpp {
class dummy_connection {
 public:
  dummy_connection();
  virtual ~dummy_connection();

 public:
  void socket(int id);
  int socket();

  int read(std::string data, int received_len);
  int write(const std::string& data, int len);

 private:
  int sock_id;
};
}  // namespace eznetpp

#endif  // INCLUDE_DUMMY_CONNECTION_H_
