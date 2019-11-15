#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace expresscpp {

class NextRouter {
 public:
  void operator()(std::shared_ptr<std::string> error = nullptr);

  void setCallback(std::function<void(std::shared_ptr<std::string>)> cb);

 private:
  std::function<void(std::shared_ptr<std::string>)> cb_;
};

typedef NextRouter next_t;

}  // namespace expresscpp
