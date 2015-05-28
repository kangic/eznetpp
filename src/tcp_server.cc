//  Copyright [2015] <kangic21@gmail.com>

#include "../include/tcp_server.h"

#include "../include/eznetpp.h"

#include "../include/tcp_connection.h"

namespace eznetpp {

const int default_port_num = 6666;
const int default_max_conn_cnt = 5000;
const int default_max_accept_cnt = 15;

tcp_server::tcp_server(void) {
  _conn_maps.clear();
}

tcp_server::~tcp_server(void) {
  _work_th.join();
}

void tcp_server::set_env(int port, int max_connections) {
  _host_port = port;
  _max_connections_cnt = max_connections;
}

int tcp_server::start_async_io() {
  logger::instance().log(logger::log_level::debug
      , "tcp_server::start_async_io() ->\n");

  // initialization
  int ret = setup_server_socket();
  if (ret != 0) {
    return ret;
  }

  ret = create_epoll_fd_and_events();
  if (ret != 0) {
    return ret;
  }

  _work_th = std::thread(&tcp_server::work_thread, this);

  if (!_work_th.joinable()) {
    logger::instance().log(logger::log_level::error
        , "tcp_server::start_async_io() - accept thread failed to create(%d)\n"
        , errno);
    return -1;
  }

  logger::instance().log(logger::log_level::debug
      , "tcp_server::start_async_io() <-\n");

  return 0;
}

void tcp_server::add_to_polling_list(connection* dc) {
  add_to_conn_maps(dc);
}

void tcp_server::add_to_conn_maps(connection *dc) {
  std::lock_guard<std::mutex> guard(_conn_maps_mutex);
  _conn_maps[dc->socket_id()] = dc;
}

void tcp_server::del_from_conn_maps(connection *dc) {
  std::lock_guard<std::mutex> guard(_conn_maps_mutex);
  _conn_maps.erase(dc->socket_id());
}

// work thread for accepting to a client
void* tcp_server::work_thread(void) {
  logger::instance().log(logger::log_level::debug
      , "tcp_server::work_thread() ->\n");

  while (1) {
    process_epoll_event(_epoll_fd, _events, _max_connections_cnt);

    usleep(10);
  }

  logger::instance().log(logger::log_level::debug
      , "tcp_server::work_thread() <-\n");

  return nullptr;
}

// create server socket and initialize for accepting
int tcp_server::setup_server_socket() {
  logger::instance().log(logger::log_level::debug
      , "tcp_server::setup_server_socket() ->\n");

  // TODO(kangic) : AF_INET6
  _server_socket = socket(AF_INET, SOCK_STREAM, 0);

  set_reuseaddr(_server_socket);

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(_host_port);

  int ret;
  ret = bind(_server_socket, (struct sockaddr *)&server_addr
      , sizeof(server_addr));

  if (ret != 0) {
    logger::instance().log(logger::log_level::error
        , "tcp_server::setup_server_socket() - bind error(%d)\n", errno);

    return errno;
  }

  ret = listen(_server_socket, default_max_accept_cnt);

  if (ret != 0) {
    logger::instance().log(logger::log_level::error
        , "tcp_server::setup_server_socket() - listen error(%d)\n", errno);

    return errno;
  }

  logger::instance().log(logger::log_level::debug
      , "tcp_server::setup_server_socket() <-\n");

  return 0;
}

// create epoll file descriptor and events
int tcp_server::create_epoll_fd_and_events() {
  // create epoll fd and event structures
  _epoll_fd = epoll_create(_max_connections_cnt);
  if (_epoll_fd == -1) {
    logger::instance().log(logger::log_level::error
        , "tcp_server::setup_server_socket() - epoll_create error(%d)\n"
        , errno);
  
    return errno;
  }

  _events = new epoll_event[_max_connections_cnt];
  if (_events == nullptr) {
    logger::instance().log(logger::log_level::error
        , "tcp_server::setup_server_socket() - failed to create events(%d)\n"
        , errno);

    return errno;
  }

  // add server_socket for listening epoll event
  int ret = add_fd(_epoll_fd, _server_socket);
  if (ret != 0) {
    logger::instance().log(logger::log_level::error
        , "tcp_server::setup_server_socket() - add_fd(ret : %d, errno : %d)\n"
        , ret, errno);

    return ret;
  }

  return 0;
}

int tcp_server::add_fd(int efd, int cfd) {
  struct epoll_event ev;

  ev.events = EPOLLIN;
  ev.data.fd = cfd;

  return epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &ev);
}

int tcp_server::del_fd(int efd, int cfd) {
  return epoll_ctl(efd, EPOLL_CTL_DEL, cfd, NULL);
}

int tcp_server::set_nonblock(int sock) {
  int flags = fcntl(sock, F_GETFL);
  flags |= O_NONBLOCK;
  return fcntl(sock, F_SETFL, flags);
}

int tcp_server::set_tcpnodelay(int sock) {
  int flag = 1;
  int result = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY
               , reinterpret_cast<char*>(&flag), sizeof(int));
  return result;
}

int tcp_server::set_reuseaddr(int sock) {
  int flag = 1;
  int result = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR
               , reinterpret_cast<char*>(&flag), sizeof(int));
  return result;
}

void tcp_server::process_epoll_event(int efd, struct epoll_event* ev
                                     , int ev_cnt) {
  logger::instance().log(logger::log_level::debug
      , "tcp_server::process_epoll_event() ->\n");
  
  int changed_events = epoll_wait(efd, ev, ev_cnt, -1);
  logger::instance().log(logger::log_level::debug
      , "tcp_server::process_epoll_event() - changed_events : %d\n"
      , changed_events);

  for (int i = 0; i < changed_events; i++) {
    if (ev[i].data.fd == _server_socket) {
      do_accept();
    } else {
      do_read(ev[i]);
    }
  }

  logger::instance().log(logger::log_level::debug
      , "tcp_server::process_epoll_event() <-\n");
}

int tcp_server::do_accept() {
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  // todo : wait for accepting a client
  int client_sock = accept(_server_socket, (struct sockaddr *)&client_addr
    , &client_addr_len);

  if (client_sock < 0) {
    logger::instance().log(logger::log_level::error
      , "tcp_server::do_accept() - failed accept()\n");

    return -1;
  }

  logger::instance().log(logger::log_level::debug
    , "tcp_server::do_accept() - client socket id : %d\n", client_sock);

  set_tcpnodelay(client_sock);

  if (add_fd(_epoll_fd, client_sock) != 0) {
    logger::instance().log(logger::log_level::error
      , "tcp_server::do_accept() - failed add_fd()\n");

    return -1;
  }

  on_accept(client_sock);

  return 0;
}

int tcp_server::do_read(struct epoll_event ev) {
  int client_fd = ev.data.fd;

  connection* dc = _conn_maps[client_fd];
  std::string msg = "";
  int len;
  int ret = dc->read_from_socket();

  if (ret <= 0) {
    dc->close_socket();
    logger::instance().log(logger::log_level::debug
                           , "tcp_server::do_read() - Close fd %d\n"
                           , client_fd);

    del_from_conn_maps(dc);
    del_fd(_epoll_fd, client_fd);

    if (dc != nullptr) {
      delete dc;
      dc = nullptr;
    }
  } else {
    logger::instance().log(logger::log_level::debug
                           , "tcp_server::do_read() - read %d bytes\n"
                           , ret);
  }
  
  return 0;
}
}  // namespace eznetpp
