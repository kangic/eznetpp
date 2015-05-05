// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_SERVER_H_
#define INCLUDE_TCP_SERVER_H_

#include <sys/epoll.h>

#include <thread>
#include <mutex>

#include <string>
#include <map>

#include "./eznetpp.h"
#include "./if_server.h"

namespace eznetpp {
class tcp_server : public if_server {
 public:
  tcp_server();
  virtual ~tcp_server();

  // override
  virtual void on_accept(int client_id) = 0;

  int start_async_io();

  void set_env(int port, int max_connections);

  void add_to_polling_list(connection* dc);

 protected:
  void* work_thread(void);

 private:
  int setup_server_socket();

  // functions to the epoll functions
  int create_epoll_fd_and_events();

  int add_fd(int efd, int cfd);
  int del_fd(int efd, int cfd);

  // functions to set the socket options
  int set_nonblock(int sock);
  int set_tcpnodelay(int sock);
  int set_reuseaddr(int sock);

  // function to call the epoll functions
  void process_epoll_event(int efd, struct epoll_event* ev, int ev_cnt);

  // functions to call the socket functions
  int do_accept();
  int do_read(struct epoll_event ev);

  // connection map functions
  void add_to_conn_maps(connection *dc);
  void del_from_conn_maps(connection *dc);

  ///////////////////////////////////
  // variables
  int _host_port;
  int _max_connections_cnt;

  int _server_socket;

  int _epoll_fd;
  struct epoll_event* _events;

  std::thread _work_th;

  // socket_id<key>, connection_class<value>
  std::map<int, connection*> _conn_maps;
  std::mutex _conn_maps_mutex;

  DISALLOW_COPY_AND_ASSIGN(tcp_server);
};
}  // namespace eznetpp

#endif  // INCLUDE_TCP_SERVER_H_
