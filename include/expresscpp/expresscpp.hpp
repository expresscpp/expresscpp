#if defined(__cpp_modules)
export module expresscpp;
export import std;
#else
#pragma once
#endif

#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "expresscpp/impl/listener.hpp"
#include "expresscpp/impl/routing_stack.hpp"
#include "expresscpp/nextrouter.hpp"
#include "expresscpp/request.hpp"
#include "expresscpp/response.hpp"
#include "expresscpp/route.hpp"
#include "expresscpp/router.hpp"
#include "expresscpp/types.hpp"

#if defined(__cpp_modules)
export namespace expresscpp {
#else
namespace expresscpp {
#endif

class ExpressCpp {
  friend class Session;

 public:
  ExpressCpp();
  ~ExpressCpp();

  template <typename T>
  void Get(std::string_view registered_path, T handler) {
    router_->Get(registered_path, handler);
  }
  template <typename T, typename... Args>
  void Get(std::string_view path, T handler, Args... args) {
    Get(path, handler);
    Get(path, args...);
  }

  template <typename T>
  void Post(std::string_view registered_path, T handler) {
    router_->Post(registered_path, handler);
  }
  template <typename T, typename... Args>
  void Post(std::string_view path, T handler, Args... args) {
    Post(path, handler);
    Post(path, args...);
  }

  template <typename T>
  void Delete(std::string_view registered_path, T handler) {
    router_->Delete(registered_path, handler);
  }
  template <typename T, typename... Args>
  void Delete(std::string_view path, T handler, Args... args) {
    Delete(path, handler);
    Delete(path, args...);
  }

  template <typename T>
  void Patch(std::string_view registered_path, T handler) {
    router_->Patch(registered_path, handler);
  }
  template <typename T, typename... Args>
  void Patch(std::string_view path, T handler, Args... args) {
    Patch(path, handler);
    Patch(path, args...);
  }

  void Error(handler_wecn_t handler);

  /*!
   * Proxy `Router#Use()` to add middleware to the app router.
   * See Router#use() documentation for details.
   *
   * If the _fn_ parameter is an express app, then it will be
   * mounted at the _route_ specified.
   */
  void Use(handler_wn_t handler);
  void Use(std::string_view path, handler_t handler);
  void Use(std::string_view path, handler_wn_t handler);
  void Use(std::string_view path, RouterPtr router);
  Router& Use(std::string_view path);

  auto GetBaseRouter();

  //! called to start listening on port
  ExpressCpp& Listen(const uint16_t port, ready_fn_cb_error_code_t callback);

  void Run();
  void Stop();

  RouterPtr GetRouter() const;
  RouterPtr GetRouter(std::string_view name) const;

  std::vector<RoutingStack> Stack() const;

  /**
   * Dispatch a req, res pair into the application. Starts pipeline processing.
   */
  void HandleRequest(request_t req, response_t res, std::function<void()> callback);

 private:
  void Init();

  std::mutex running_mtx;
  std::condition_variable running_cv;
  bool finished_{false};
  std::unique_ptr<Router> router_;
  std::shared_ptr<Listener> listener_;
  std::vector<Route> routes_;
  std::size_t threads_{4u};
  bool error_handler_registered_{false};
  handler_wecn_t error_handler_;
  bool listening_{false};
};

}  // namespace expresscpp
