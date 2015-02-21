#ifndef _EZNETPP_TEST_USR_SERVER_H_
#define _EZNETPP_TEST_USR_SERVER_H_

#include "../include/dummy_server_socket.h"

class usr_server : public eznetpp::dummy_server_socket {
public:
	 usr_server();
	 usr_server(const std::string ip, unsigned int port, unsigned int max_accepts);
	 virtual ~usr_server();
  
public:
	// override
  void on_accept();
  void on_read();
  void on_write();

private:
	dummy_server_socket svr;
};
  
#endif  // _EZNETPP_TEST_USR_SERVER_H_
