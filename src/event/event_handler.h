// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_EVENT_HANDLER_H_
#define INCLUDE_EVENT_HANDLER_H_

#include <eznetpp.h>

namespace eznetpp {
namespace event {
class eznetpp::net::connection;
class event;
class event_handler {
 public:
  event_handler(void);
  virtual ~event_handler(void);

  virtual void on_accept(int fd, int err_no) = 0;

  virtual void on_recv(const std::string& msg, int len, int err_no) = 0;
  virtual void on_send(unsigned int len, int err_no) = 0;
  virtual void on_close(int err_no) = 0;

 protected:
  void set_flag(const event_type type);

 private:
  event_type _type;

  DISALLOW_COPY_AND_ASSIGN(event_handler);
};
}  // namespace event
}  // namespace eznetpp
#endif INCLUDE_EVENT_HANDLER_H_
