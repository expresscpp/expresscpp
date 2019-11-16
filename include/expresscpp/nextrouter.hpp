#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace expresscpp {

class Router;
class Request;
class Response;

class NextRouter {
 public:
  NextRouter() = default;
  NextRouter(Router* router, std::shared_ptr<Request> req, std::shared_ptr<Response> res);
  void operator()(std::shared_ptr<std::string> error = nullptr);

 private:
  std::shared_ptr<Request> req_;
  std::shared_ptr<Response> res_;
  Router* router_{nullptr};
};

typedef NextRouter next_t;

}  // namespace expresscpp
