// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_EVENT_HANDLER_H_
#define INCLUDE_EVENT_HANDLER_H_

#include "eznetpp.h"
#include "net/if_socket.h"
#include "net/tcp/tcp_socket.h"

namespace eznetpp {
namespace event {

class event_handler {
 public:
  event_handler(void) = default;
  virtual ~event_handler(void) = default;

  enum event_handler_type {
    socket = 0,
    acceptor,
  };
  event_handler_type type() { return _handler_type; };
  virtual void on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no) = 0;
  virtual void on_connect(int err_no) = 0;
  virtual void on_recv(const std::string& msg, int len, int err_no) = 0;
  virtual void on_send(unsigned int len, int err_no) = 0;
  virtual void on_close(int err_no) = 0;

 protected:
  event_handler_type _handler_type;
};

class socket_event_handler : public event_handler {
 public:
  socket_event_handler(void) { 
    _handler_type = event_handler_type::socket;
  };
  virtual ~socket_event_handler(void) = default;

  virtual void on_connect(int err_no) = 0;
  virtual void on_recv(const std::string& msg, int len, int err_no) = 0;
  virtual void on_send(unsigned int len, int err_no) = 0;
  virtual void on_close(int err_no) = 0;

 protected:
  void on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no){};
};

class acceptor_event_handler : public event_handler {
 public:
  acceptor_event_handler(void) {
    _handler_type = event_handler_type::acceptor;
  };
  virtual ~acceptor_event_handler(void) = default;

  virtual void on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no) = 0;
  virtual void on_close(int err_no) = 0;

 protected:
  void on_connect(int err_no){};
  void on_recv(const std::string& msg, int len, int err_no){};
  void on_send(unsigned int len, int err_no){};
};

}  // namespace event
}  // namespace eznetpp
#endif  // INCLUDE_EVENT_HANDLER_H_
