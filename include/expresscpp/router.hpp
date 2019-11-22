#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "boost/uuid/uuid.hpp"
#include "expresscpp/console.hpp"
#include "expresscpp/layer.hpp"
#include "expresscpp/request.hpp"
#include "expresscpp/response.hpp"
#include "expresscpp/route.hpp"
#include "expresscpp/types.hpp"

namespace expresscpp {

class Router {
 public:
  Router();
  Router(std::string_view router_name);

  //! @brief handler for all routes matching this path
  void Use(std::string_view path, handler_t handler);

  //! @brief registering a router to serve path and subpaths of this path
  void Use(std::string_view path, std::shared_ptr<Router> router);

  /*!
   * Proxy `Router#Use()` to add middleware to the app router.
   * See Router#use() documentation for details.
   *
   * If the _fn_ parameter is an express app, then it will be
   * mounted at the _route_ specified.
   */
  void Use(std::string_view path, handler_wn_t handler);
  void Use(handler_wn_t handler);

  template <typename T>
  Router& Get(std::string_view path, T handler) {
    Console::Debug(fmt::format("get \"{}\" registered", path));
    RegisterPath(path, HttpMethod::Get, handler);
    return *this;
  }

  template <typename T>
  Router& Put(std::string_view path, T handler) {
    Console::Debug(fmt::format("put \"{}\" registered", path));
    RegisterPath(path, HttpMethod::Put, handler);
    return *this;
  }

  template <typename T>
  Router& Post(std::string_view path, T handler) {
    Console::Debug(fmt::format("post \"{}\" registered", path));
    RegisterPath(path, HttpMethod::Post, handler);
    return *this;
  }

  template <typename T>
  Router& Patch(std::string_view path, T handler) {
    Console::Debug(fmt::format("patch \"{}\" registered", path));
    RegisterPath(path, HttpMethod::Patch, handler);
    return *this;
  }

  template <typename T>
  Router& Delete(std::string_view path, T handler) {
    Console::Debug(fmt::format("delete \"{}\" registered", path));
    RegisterPath(path, HttpMethod::Delete, handler);
    return *this;
  }

  template <typename T>
  Router& Get(T handler) {
    return Get("/", handler);
  }

  template <typename T>
  Router& Put(T handler) {
    return Put("/", handler);
  }

  template <typename T>
  Router& Post(T handler) {
    return Post("/", handler);
  }

  template <typename T>
  Router& Delete(T handler) {
    return Delete("/", handler);
  }

  template <typename T>
  Router& Patch(T handler) {
    return Patch("/", handler);
  }

  template <typename T>
  void RegisterPath(std::string_view registered_path, const HttpMethod method, T handler) {
    auto route = CreateRoute(registered_path);
    Console::Debug(fmt::format("registering path \"{}\"", registered_path));
    PathToRegExpOptions op;
    auto layer = std::make_shared<Layer>("/", op, parent_path_, handler);
    layer->SetMethod(method);
    route->methods_.insert(method);
    route->stack_.emplace_back(layer);
  }

  void RegisterPath(std::string_view registered_path, const HttpMethod method, handler_wn_t handler);

  void HandleRequest(std::shared_ptr<Request> req, std::shared_ptr<Response> res);

  void SetParentPath(const std::string& parent_path);

  //! @brief returns a router which then can use used to serve some paths
  auto GetRouter();
  auto GetRouter(std::string_view name);

  /**
   * Create a new Route for the given path.
   *
   * Each route contains a separate middleware stack and VERB handlers.
   *
   * See the Route api documentation for details on adding handlers
   * and middleware to routes.
   *
   * @param {String} registered_path
   * @return {Route}
   */
  std::shared_ptr<Route> CreateRoute(const std::string_view registered_path);

  std::string_view GetName() const;

  std::vector<std::shared_ptr<Layer>> stack() const;

  void Next(std::shared_ptr<Request> req, std::shared_ptr<Response> res, std::shared_ptr<std::string> err = nullptr);

 private:
  void Init();

  //!
  /* @brief Enable case sensitivity.
   * Disabled by default, treating “/Foo” and “/foo” as the same.
   * @ref https://expressjs.com/en/api.html#express.router
   */
  bool caseSensitive{false};

  //!
  /* @brief Preserve the req.params values from the parent router.
   * If the parent and the child have conflicting param names,
   * the child’s value take precedence.
   * @ref https://expressjs.com/en/api.html#express.router
   */
  bool mergeParams{false};

  //!
  /* @brief Enable strict routing.
   * Disabled by default, “/foo” and “/foo/” are treated the same by the router.
   * @ref https://expressjs.com/en/api.html#express.router
   */
  bool strict{false};

  std::vector<Route> routes_;

  //! @brief uuid of the router, for debugging
  boost::uuids::uuid uuid_;

  //! @brief creation date of the router, for debugging
  std::chrono::system_clock::time_point timestamp_;

  //! @brief name of the router, mostly for debugging
  std::string_view name_{"unknown"};

  std::vector<std::shared_ptr<Layer>> stack_;

  std::string parent_path_;
};

typedef std::shared_ptr<Router> RouterPtr;

}  // namespace expresscpp
