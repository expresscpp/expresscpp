#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "boost/algorithm/string.hpp"
#include "boost/uuid/uuid.hpp"
#include "magic_enum.hpp"

#include "expresscpp/request.hpp"
#include "expresscpp/response.hpp"
#include "expresscpp/types.hpp"

struct Route {
  std::string_view path_;
  HttpMethod method_;

  auto getMethodName() const {
    const std::string_view m = magic_enum::enum_name(method_);
    const std::string mm(m);
    return boost::to_upper_copy<std::string>(mm);
  }
};

class Router {
 public:
  Router();
  Router(std::string_view name);

  void Use(std::string_view path, express_handler_t handler) {
    std::cout << "use " << path << " called" << std::endl;
    RegisterPath(path, HttpMethod::All, handler);
  }
  void Get(std::string_view path, express_handler_t handler) {
    std::cout << "get " << path << " called" << std::endl;
    RegisterPath(path, HttpMethod::Get, handler);
  }
  void Post(std::string_view path, express_handler_t handler) {
    std::cout << "post " << path << " called" << std::endl;
    RegisterPath(path, HttpMethod::Post, handler);
  }
  void Delete(std::string_view path, express_handler_t handler) {
    std::cout << "delete " << path << " called" << std::endl;
    RegisterPath(path, HttpMethod::Delete, handler);
  }

  void Use(std::string_view path, std::shared_ptr<Router> router) {
    std::cout << "adding router to path: " << path << std::endl;
    routers.push_back({path, router});
    RegisterPath(path, HttpMethod::All,
                 [&](auto req, auto res) { router->HandleRequest(req, res); });
  }

  void RegisterPath(std::string_view path, HttpMethod method, express_handler_t handler) {
    std::cout << "handler registered for expresscpp path " << path << " and method "
              << magic_enum::enum_name(method) << std::endl;

    Route r{path, method};
    routes_.push_back(r);

    ExpressCppHandler express_cpp_handler;
    express_cpp_handler.method = method;
    express_cpp_handler.debug_function_name = typeid(handler).name();

    express_cpp_handler.handler = handler;
    handler_map_[path].push(express_cpp_handler);
    // TODO: handle path = "*" -> always call this handler e.g. logger
  }

  void HandleRequest(std::shared_ptr<Request> req, std::shared_ptr<Response> res) {
    std::cout << "router HandleRequest" << std::endl;

    assert(req != nullptr);
    assert(res != nullptr);

    std::cout << "handling request for path: \"" << req->path_ << "\" and method \""
              << magic_enum::enum_name(req->method_) << "\"" << std::endl;

    for (const auto& h : handler_map_) {
      std::cout << "trying path " << h.first << std::endl;

      const std::string_view requested_path = req->path_.data();
      const std::string_view registered_path = h.first;
      std::cout << requested_path << " : " << registered_path << std::endl;

      if (requested_path == registered_path) {
        std::cout << "path is registered" << std::endl;

        auto tmp_q = h.second;
        while (!tmp_q.empty()) {
          auto q_element = tmp_q.front();
          if (req->method_ == q_element.method) {
            const auto method_name = magic_enum::enum_name(q_element.method);
            std::cout << "method " << method_name << " is registered" << std::endl;

            auto handler = q_element.handler;
            handler(req, res);
            return;
          }
          tmp_q.pop();
        }

        if (h.second.front().method == HttpMethod::All) {
          std::cout << "using all method" << std::endl;
          auto handler = h.second.front().handler;
          handler(req, res);
          return;
        }
      }
    }

    auto f = handler_map_.find("*");
    if (f == handler_map_.end()) {
      std::cout << "no wildcard defined, and path not registered, doing nothing" << std::endl;
    } else {
      std::cout << "using the wildcard for serving this path" << std::endl;
      f->second.front().handler(req, res);
      return;
    }
  }

  auto GetRouter() {
    std::cout << "getting a router" << std::endl;
    auto r = std::make_shared<Router>();
    return r;
  }

  auto GetRouter(std::string_view name) {
    std::cout << "getting a router" << std::endl;
    auto r = std::make_shared<Router>(name);
    return r;
  }

  void printRoutes() {
    static int intendation = 0;
    intendation++;

    // loop through all routes for this router
    for (const auto& v : routes_) {
      std::string a = "";
      for (int i = 0; i < intendation * 4; i++) {
        a += " ";
      }

      // print this route for this router
      std::cout << a << v.getMethodName() << ": \"" << v.path_ << "\"" << std::endl;

      for (const auto& rs : routers) {
        if (rs.first != "" && rs.first == v.path_) {
          //          std::cout << "subpath is " << rs.first << std::endl;
          rs.second->printRoutes();
        }
      }
    }
    //    if (r.second->routers.size() > 0) {
    //      //    std::cout << "router has subrouters" << std::endl;
    //      for (const auto& subrouter : r.second->routers) {
    //        printRouters(subrouter, r.second->routers);
    //      }
    //    }

    intendation--;
  }

  std::vector<Route> routes_;
  std::vector<std::pair<std::string_view, std::shared_ptr<Router>>> routers;
  std::map<std::string_view, express_handler_queue_t> handler_map_;

  std::string_view name_;
  boost::uuids::uuid uuid_;
  std::chrono::system_clock::time_point timestamp_;
};

typedef std::shared_ptr<Router> RouterPtr;
