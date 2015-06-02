# eznetpp

## Introduction

This is a network library for C++. It was developed by using C++11.


## Purpose

* Can develop a network application easy.
* Can concentrate to develop own business logic.


## Getting started

* Clone this repository
  > git clone https://github.com/kangic/eznetpp/eznetpp.git

* Build a library
  * Create output files in `{EZNETPP_ROOT}/build` directory after run a `cmake . && make` command.
  

## How to use

* TCP Server
  1. Make a server class. It should be inherite a `tcp_server` class.
    - Override a `on_accept` function and implement it.
	- Create a connection class and push it to the list.
  2. Make a connection class. It should be inherite a `tcp_connection` class.
    - Override `on_recv` `on_send` `on_close` functions and implement them.
  3. Add a server class to the `runner` and then call the `run` function in your main program. That's it.
  
* TCP Client
  1. Make a client class. It should be inherite a `tcp_client` class.
    - Override a `on_connect` function and implement it.
	- Create a connection class and push it to the list.
  2. Make a connection class. It should be inherite a `tcp_connection` class.
    - Override `on_recv` `on_send` `on_close` functions and implement them.
  3. Add a server class to the `runner` and then call the `run` function in your main program. That's it.


## Examples List

* TCP Echo Server / Client
