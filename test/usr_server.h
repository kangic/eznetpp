#ifndef TEST_USR_SERVER_H_
#define TEST_USR_SERVER_H_

#include <string>

#include "../include/tcp_server.h"

class usr_server : public eznetpp::tcp_server {
 public:
  usr_server();
  usr_server(int port, int max_accepts);
  virtual ~usr_server();

 public:
  // override
  void on_accept(const eznetpp::connection& conn, int err_no);
  void on_read(const eznetpp::connection& conn, const std::string& msg
      , int len, int err_no);
  void on_write(const eznetpp::connection& conn, unsigned int len
      , int err_no);
  void on_close(const eznetpp::connection& conn, int err_no);
};

#endif  // TEST_USR_SERVER_H_
