#include "expresscpp/expresscpp.hpp"

#include <algorithm>
#include <condition_variable>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "expresscpp/console.hpp"
#include "expresscpp/impl/listener.hpp"
#include "expresscpp/impl/routing_stack.hpp"
#include "expresscpp/impl/session.hpp"
#include "expresscpp/route.hpp"
#include "nlohmann/json.hpp"

namespace expresscpp {

ExpressCpp::ExpressCpp() {
  Init();
}

ExpressCpp::~ExpressCpp() {
  if (listener_) {
    listener_->Stop();
  }
  Console::Debug("ExpressCpp destroyed");
}

void ExpressCpp::Error(handler_wecn_t handler) {
  error_handler_ = handler;
  Console::Debug("error handler registered");
  error_handler_registered_ = true;
}

void ExpressCpp::Use(handler_wn_t handler) {
  Console::Debug("using handler for all paths");
  router_->Use(handler);
}

void ExpressCpp::Use(std::string_view registered_path, handler_t handler) {
  (void)registered_path;
  (void)handler;
  //  RegisterPath(registered_path, HttpMethod::All, handler);
  throw std::runtime_error("not implemented yet");
}

void ExpressCpp::Use(std::string_view registered_path, handler_wn_t handler) {
  router_->RegisterPath(registered_path, HttpMethod::All, handler);
}

void ExpressCpp::Use(std::string_view registered_path, RouterPtr router) {
  Console::Debug(fmt::format(R"(adding router "{}" to path "{}")", router->GetName(), registered_path));
  router_->Use(registered_path, router);
}

Router& ExpressCpp::Use(std::string_view path) {
  auto router = GetRouter();
  router_->Use(path, router);
  return *router;
}

ExpressCpp& ExpressCpp::Listen(const uint16_t port, ready_fn_cb_error_code_t callback) {
  std::error_code ec;
  if (listening_) {
    Console::Error("already listening");
    ec = std::make_error_code(std::errc::already_connected);
    callback(ec);
    return *this;
  }

  const std::string ip_addr = "0.0.0.0";

  // Create and launch a listening port
  listener_ = std::make_shared<Listener>(ip_addr, port, this, [&](auto listen_ec) {
    if (listen_ec) {
      ec = listen_ec;
      return;
    }
  });

  if (ec) {
    callback(ec);
    return *this;
  }

  listener_->run();
  listening_ = true;

  listener_->launch_threads();

  callback(ec);
  return *this;
}

void ExpressCpp::Run() {
  std::unique_lock<std::mutex> lock(running_mtx);
  while (!finished_) {
    running_cv.wait(lock);
  }
}

void ExpressCpp::Stop() {
  std::unique_lock<std::mutex> lock(running_mtx);
  finished_ = true;
  running_cv.notify_all();
}

RouterPtr ExpressCpp::GetRouter() const {
  Console::Debug("getting a router");
  auto r = std::make_shared<Router>();
  return r;
}

RouterPtr ExpressCpp::GetRouter(std::string_view name) const {
  Console::Debug(fmt::format("getting a router{}", name));
  auto r = std::make_shared<Router>(name);
  return r;
}

void ExpressCpp::HandleRequest(request_t req, response_t res, std::function<void()> callback) {
  assert(req != nullptr);
  assert(res != nullptr);

  try {
    if (callback == nullptr) {
      callback = []() {
        // TODO(gocarlos): print here the callstack...
        Console::Error("ERROR");
      };
    }

    if (router_ == nullptr) {
      Console::Error("no routes defined on app");
      callback();
      return;
    }

    Console::Debug(fmt::format(R"(handling request: "{}" "{}")", getHttpMethodName(req->getMethod()), req->getPath()));

    router_->HandleRequest(req, res);
  } catch (...) {
    auto eptr = std::current_exception();
    if (error_handler_registered_) {
      expresscpp::NextRouter next;
      auto ec = std::make_error_code(std::errc::bad_message);
      error_handler_(ec, req, res, next);
    } else {
      std::rethrow_exception(eptr);
    }
  }
}

std::vector<RoutingStack> ExpressCpp::Stack() const {
  std::vector<RoutingStack> routing_stack;

  for (const auto& l : router_->stack()) {
    // if route is nullptr -> this means it is a middleware
    if (l->GetRoute() != nullptr) {
      for (const auto& ll : l->GetRoute()->stack_) {
        Console::Debug(fmt::format(R"(registered paths: "{}" "{}")", l->GetRoute()->GetPath(),
                                   getHttpMethodName(ll->GetMethod())));

        RoutingStack rs{ l->GetRoute()->GetPath().data(), ll->GetMethod()};
        routing_stack.push_back(rs);
      }
    } else {
      RoutingStack rs{l->GetPath().data(), l->GetMethod()};
      routing_stack.push_back(rs);
    }
  }
  return routing_stack;
}

void ExpressCpp::Init() {
  Console::Debug("ExpressCpp created");
  router_ = std::make_unique<Router>("base router");
  finished_ = false;
}

}  // namespace expresscpp
