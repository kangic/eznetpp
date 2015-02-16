#ifndef _EZNETPP_TEST_USR_SERVER_H_
#define _EZNETPP_TEST_USR_SERVER_H_

#include "../include/dummy_server_socket.h"

class usr_server : public eznetpp::dummy_server_socket {
 public:
	 usr_server();
	 usr_server(const std::string ip, unsigned int port, unsigned int max_accepts);
	 virtual ~usr_server();
  
 public:
  void handle_accept();
  void handle_read();
  void handle_write();

	void set_env(const std::string& ip, unsigned int port, unsigned int max_accepts);

 private:
	dummy_server_socket svr;
};
  
#endif  // _EZNETPP_TEST_USR_SERVER_H_
