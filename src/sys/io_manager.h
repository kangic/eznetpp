// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IO_MANAGER_H_
#define INCLUDE_IO_MANAGER_H_

#include <eznetpp.h>

namespace eznetpp {
namespace sys {
class socket;
class io_manager {
 public:
  io_manager(void);
  virtual ~io_manager(void);

  void add_to_polling_list(int fd);
  void del_from_polling_list(int fd);

 protected:
  void* poller_thread(void);
 
 private:
  std::thread _poller_th;

  std::vector<int> _polling_list;

  DISALLOW_COPY_AND_ASSIGN(io_manager);
};
}  // namespace sys
}  // namespace eznetpp

#endif INCLUDE_IO_MANAGER_H_
