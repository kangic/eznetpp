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

event_type type()
const std::string& data()
int data_length()
int err_no()
}  // namespace eznetpp
