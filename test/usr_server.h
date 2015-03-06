#ifndef TEST_USR_SERVER_H_
#define TEST_USR_SERVER_H_

#include <string>

#include "../include/dummy_server.h"

class usr_server : public eznetpp::dummy_server {
 public:
  usr_server();
  usr_server(const std::string ip, unsigned int port, unsigned int max_accepts);
  virtual ~usr_server();

 public:
  // override
  void on_accept(const eznetpp::dummy_connection& conn, int err_no);
  void on_read(const eznetpp::dummy_connection& conn, const std::string& msg
      , int len, int err_no);
  void on_write(const eznetpp::dummy_connection& conn, unsigned int len
      , int err_no);
  void on_close(const eznetpp::dummy_connection& conn, int err_no);
};

#endif  // TEST_USR_SERVER_H_
