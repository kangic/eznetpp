// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_EVENT_HANDLER_H_
#define INCLUDE_EVENT_HANDLER_H_

#include "eznetpp.h"

namespace eznetpp {
namespace event {

class event_handler {
 public:
  event_handler(void) = default;
  virtual ~event_handler(void) = default;

  enum event_handler_type {
    acceptor = 0,
    connector,
  };
  event_handler_type handler_type() { return _handler_type; };

 protected:
  event_handler_type _handler_type;
};

class acceptor_event_handler : public event_handler {
 public:
  acceptor_event_handler(void) {
    _handler_type = event_handler_type::acceptor;
  };
  virtual ~acceptor_event_handler(void) = default;

  virtual void on_accept(int fd, int err_no) = 0;
  virtual void on_close(int err_no) = 0;
};

class connector_event_handler : public event_handler {
 public:
  connector_event_handler(void) { 
    _handler_type = event_handler_type::connector;
  };
  virtual ~connector_event_handler(void) = default;

  virtual void on_connect(int err_no) = 0;
  virtual void on_recv(const std::string& msg, int len, int err_no) = 0;
  virtual void on_send(unsigned int len, int err_no) = 0;
  virtual void on_close(int err_no) = 0;
};

}  // namespace event
}  // namespace eznetpp
#endif  // INCLUDE_EVENT_HANDLER_H_
