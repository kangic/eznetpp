// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_SERVER_H_
#define INCLUDE_TCP_SERVER_H_

#include "eznetpp.h"
#include "event/event_handler.h"
#include "net/if_server.h"
#include "sys/io_manager.h"

namespace eznetpp {
namespace net {
class connection;
class tcp_server : public if_server, public event_handler {
 public:
  tcp_server();
  virtual ~tcp_server();

  int start_async_io();

  void set_env(int port, int max_connections, int num_of_worker_threads);

  void add_to_connection_list(connection* conn);

 protected:
  void* acceptor_thread(void);
  void* worker_thread(void);

 private:
  int setup_server_socket();

  // functions to set the socket options
  int set_nonblock(int sock);
  int set_tcpnodelay(int sock);
  int set_reuseaddr(int sock);

  // function to call the epoll functions
  void process_epoll_event(int efd, struct epoll_event* ev, int ev_cnt);

  // functions to call the socket functions
  int do_accept();
  //int do_read(struct epoll_event ev);
  int do_read(int client_fd);

  // connection map functions
  void add_to_conn_maps(connection *conn);
  void del_from_conn_maps(connection *conn);

  ///////////////////////////////////
  // variables
  int _host_port = 6666;
  int _num_of_worker_threads = 2;

  int _server_socket = -1;

  std::thread _acceptor_th;  // TODO(kangic) : accept to client connection
  std::vector<std::thread> _worker_th_vec{};
  
  std::condition_variable _acceptor_cv;
  std::mutex _acceptor_mutex;
  
  std::condition_variable _worker_cv;
  std::mutex _worker_mutex;
  
  // socket_id<key>, connection_class<value>
  std::map<int, connection*> _conn_maps;
  std::mutex _conn_maps_mutex;

  DISALLOW_COPY_AND_ASSIGN(tcp_server);
};
}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_TCP_SERVER_H_
