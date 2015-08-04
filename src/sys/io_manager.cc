//  Copyright [2015] <kangic21@gmail.com>

#include "io_manager.h"

io_manager::io_manager(void) {

}

io_manager::~io_manager(void) {

}

bool io_manager::add_fd(int fd) {
  _polling_list.push_back(fd);
  return true;
}

bool io_manager::del_fd(int fd) {
  for (auto iter : _polling_list) {
    if (*iter == fd) {
      iter = _polling_list.erase(iter);
      return true;
    }
  }

  // todo : print warning log

  return false;
}

int io_manager::send_data(int fd, const std::string& data, int len) {
  int send_bytes = send(fd, data.c_str(), len, MSG_NOSIGNAL);
}

void io_manager::loop(void) {

}

void* io_manager::read_loop(void) {

}
 
