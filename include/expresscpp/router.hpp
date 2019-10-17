#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "expresscpp/request.hpp"
#include "expresscpp/response.hpp"

class Router {
 public:
  template <typename F>
  void Use(std::string path, F f) {
    std::cout << "use " << path << " called" << std::endl;
  }
  template <typename F>
  void Get(std::string path, F f) {
    std::cout << "get " << path << " called" << std::endl;
  }

  template <typename F>
  void Post(std::string path, F f) {
    std::cout << "post " << path << " called" << std::endl;
  }
  template <typename F>
  void Delete(std::string path, F f) {
    std::cout << "delete " << path << " called" << std::endl;
  }

  void HandleRequest(std::shared_ptr<Request> req, std::shared_ptr<Response> res) {
    std::cout << "router HandleRequest" << std::endl;
  }

 private:
  //    std::map<std::string, F> routing_table;
};
