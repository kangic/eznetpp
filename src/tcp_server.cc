//  Copyright [2015] <kangic21@gmail.com>

#include <tcp_server.h>

#include <tcp_connection.h>

namespace eznetpp {

const int default_max_accept_cnt = 15;

tcp_server::tcp_server(void) {
  _conn_maps.clear();
}

tcp_server::~tcp_server(void) {
  _poller_th.join();
  _acceptor_th.join();

  std::for_each(_worker_th_vec.begin(), _worker_th_vec.end()
                , [](std::thread& thr) {
    thr.join();
  });
}

////////////////////////////////////////////////////////////////////////////////
// public functions
int tcp_server::start_async_io() {
  // initialization
  int ret = setup_server_socket();
  if (ret != 0) {
    return ret;
  }

  ret = create_epoll_fd_and_events();
  if (ret != 0) {
    return ret;
  }

  // add server_socket for listening epoll event
  ret = add_fd(_epoll_fd, _server_socket);
  if (ret != 0) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "add_fd(ret : %d, errno : %d)"
                           , ret, errno);

    return ret;
  }

  _poller_th = std::thread(&tcp_server::poller_thread, this);

  if (!_poller_th.joinable()) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "failed to create poller thread(%d)"
                           , errno);
    return -1;
  }

  _acceptor_th = std::thread(&tcp_server::acceptor_thread, this);

  if (!_acceptor_th.joinable()) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "failed to create acceptor thread(%d)"
                           , errno);
    return -1;
  }

  // TODO(kangic) : measure number of worker threads(from user?)
  for (int i = 0; i < _num_of_worker_threads; ++i) {
    _worker_th_vec.emplace_back(std::thread(&tcp_server::worker_thread, this));
  }

  return 0;
}

void tcp_server::set_env(int port, int max_connections
                         , int num_of_worker_threads) {
  _host_port = port;
  _max_connections_cnt = max_connections;
  _num_of_worker_threads = num_of_worker_threads;
  
}

void tcp_server::add_to_connection_list(connection* conn) {
  add_to_conn_maps(conn);
}

////////////////////////////////////////////////////////////////////////////////
// poller thread for polling to fds
void* tcp_server::poller_thread(void) {
  logger::instance().log(logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "start poller_thread");

  while (1) {
    process_epoll_event(_epoll_fd, _events, _max_connections_cnt);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return nullptr;
}

void* tcp_server::acceptor_thread(void) {
  logger::instance().log(logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "start acceptor_thread");

  while (1) {
    std::unique_lock<std::mutex> lk(_acceptor_mutex);
    _acceptor_cv.wait(lk);

    do_accept();
  }

  return nullptr;
}

void* tcp_server::worker_thread(void) {
  logger::instance().log(logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "start worker_thread");

  int event_fd = -1;
  while (1) {
    {
      std::unique_lock<std::mutex> lk(_worker_mutex);
      _worker_cv.wait(lk);

      // pop event fd number from the vector
      //do_read(ev[i]);
      event_fd = _received_event_fd;
    }

    do_read(event_fd);
  }

  return nullptr;
}


////////////////////////////////////////////////////////////////////////////////
// create server socket and initialize to accept the client
int tcp_server::setup_server_socket() {
  // TODO(kangic) : AF_INET6
  _server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_server_socket == -1) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "socket() error(%d)", errno);

    return -1;
  }

  set_reuseaddr(_server_socket);

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(_host_port);

  int ret;
  ret = bind(_server_socket, (struct sockaddr *)&server_addr
      , sizeof(server_addr));

  if (ret != 0) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "bind error(%d)", errno);

    return errno;
  }

  ret = listen(_server_socket, default_max_accept_cnt);

  if (ret != 0) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "listen error(%d)", errno);

    return errno;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// create epoll file descriptor and events
int tcp_server::create_epoll_fd_and_events() {
  // create epoll fd and event structures
  _epoll_fd = epoll_create(_max_connections_cnt);
  if (_epoll_fd == -1) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "epoll_create error(%d)"
                           , errno);
  
    return errno;
  }

  _events = new epoll_event[_max_connections_cnt];
  if (_events == nullptr) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "failed to create events(%d)"
                           , errno);

    return errno;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// add / delete fd to epoll_ctl
int tcp_server::add_fd(int efd, int cfd) {
  struct epoll_event ev;

  ev.events = EPOLLIN;
  ev.data.fd = cfd;

  return epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &ev);
}

int tcp_server::del_fd(int efd, int cfd) {
  return epoll_ctl(efd, EPOLL_CTL_DEL, cfd, NULL);
}

////////////////////////////////////////////////////////////////////////////////
// set socket options
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

////////////////////////////////////////////////////////////////////////////////
// observe to the epoll file descriptor and call the functions
void tcp_server::process_epoll_event(int efd, struct epoll_event* ev
                                     , int ev_cnt) {
  int changed_events = epoll_wait(efd, ev, ev_cnt, -1);
  logger::instance().log(logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "changed_events : %d"
                         , changed_events);

  for (int i = 0; i < changed_events; i++) {
    if (ev[i].data.fd == _server_socket) {
      //do_accept();
      {
        std::unique_lock<std::mutex> lk(_worker_mutex);
      }
      _acceptor_cv.notify_one();
    } else {
      //do_read(ev[i]);
      {
        std::unique_lock<std::mutex> lk(_worker_mutex);
        _received_event_fd = ev[i].data.fd;
      }
      _worker_cv.notify_one();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// functions to call the socket functions
int tcp_server::do_accept() {
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  // todo : wait to accept a client connection
  int client_sock = accept(_server_socket, (struct sockaddr *)&client_addr
    , &client_addr_len);

  if (client_sock < 0) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "failed accept()");

    return -1;
  }

  logger::instance().log(logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "client socket id : %d", client_sock);

  set_tcpnodelay(client_sock);

  if (add_fd(_epoll_fd, client_sock) != 0) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "failed add_fd()");

    return -1;
  }

  on_accept(client_sock);

  return 0;
}

//int tcp_server::do_read(struct epoll_event ev) {
int tcp_server::do_read(int client_fd) {
  //int client_fd = ev.data.fd;

  connection* conn = _conn_maps[client_fd];

  int ret = conn->recv_from_socket();

  if (ret <= 0) {
    conn->close_socket();
    logger::instance().log(logger::log_level::debug
                           , __FILE__, __FUNCTION__, __LINE__
                           , "Close fd %d"
                           , client_fd);

    del_from_conn_maps(conn);
    del_fd(_epoll_fd, client_fd);

    if (conn != nullptr) {
      delete conn;
      conn = nullptr;
    }
  } else {
    logger::instance().log(logger::log_level::debug
                           , __FILE__, __FUNCTION__, __LINE__
                           , "read %d bytes"
                           , ret);
  }
  
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// connection map functions
void tcp_server::add_to_conn_maps(connection *conn) {
  std::lock_guard<std::mutex> guard(_conn_maps_mutex);
  _conn_maps[conn->socket_id()] = conn;
}

void tcp_server::del_from_conn_maps(connection *conn) {
  std::lock_guard<std::mutex> guard(_conn_maps_mutex);
  _conn_maps.erase(conn->socket_id());
}

}  // namespace eznetpp
