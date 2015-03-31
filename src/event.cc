// Copyright [2015] <kangic21@gmail.com>

#include "../include/event.h"

namespace eznetpp {

event::event() {
}

event::~event() {
}

void event::set(event_type type, const std::string& data, int data_len
    , int err_no) {
  _type = type;
  _data = data;
  _data_len = data_len;
  _err_no = err_no;
}

event_type event::type() {
  return type;
}

const std::string& event::data() {
  return data;
}

int event::data_length() {
  return data_len;
}

int event::err_no() {
  return err_no;
}

}  // namespace eznetpp
