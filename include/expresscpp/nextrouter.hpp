#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "expresscpp/console.hpp"

namespace expresscpp {

class NextRouter {
 public:
  void operator()(std::shared_ptr<std::string> error = nullptr) {
    std::cout << "next called" << std::endl;
    if (error) {
      Console::Error(*error);
      cb_(error);
    }
  }

  void setCallback(std::function<void(std::shared_ptr<std::string>)> cb) { cb_ = cb; }

 private:
  std::function<void(std::shared_ptr<std::string> error)> cb_;
};

typedef std::shared_ptr<NextRouter> express_next_t;

}  // namespace expresscpp
