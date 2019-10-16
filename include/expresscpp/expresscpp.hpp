#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "boost/asio.hpp"
#include "magic_enum.hpp"

#include "expresscpp/impl/listener.hpp"
#include "expresscpp/impl/session.hpp"
#include "expresscpp/nextrouter.hpp"
#include "expresscpp/request.hpp"
#include "expresscpp/response.hpp"
#include "expresscpp/router.hpp"
#include "expresscpp/types.hpp"

const auto threads = std::max<int>(1, std::atoi("4"));

class ExpressCpp {
 public:
  ExpressCpp();
  ~ExpressCpp();

  void Get(std::string path, express_handler_t handler) {
    RegisterPath(path, HttpMethod::Get, handler);
  }
  void Post(std::string path, express_handler_t handler) {
    RegisterPath(path, HttpMethod::Post, handler);
  }
  void Delete(std::string path, express_handler_t handler) {
    RegisterPath(path, HttpMethod::Delete, handler);
  }
  void Patch(std::string path, express_handler_t handler) {
    RegisterPath(path, HttpMethod::Patch, handler);
  }

  template <typename HandlerType>
  void Use(std::string path, HandlerType handler) {
    RegisterPath(path, HttpMethod::All, handler);
  }

  template <typename Callback>
  ExpressCpp& Listen(uint16_t port, Callback callback) {
    const auto address = boost::asio::ip::make_address("0.0.0.0");

    // Create and launch a listening port
    listener_ = std::make_shared<Listener>(
        ioc, boost::asio::ip::tcp::endpoint{address, port}, this);
    listener_->run();

    // Run the I/O service on the requested number of threads
    io_threads.reserve(threads);
    for (auto i = threads; i > 0; --i) {
      io_threads.emplace_back([this] { ioc.run(); });
    }
    callback();
    return *this;
  }

  // FIXME: quick and dirty
  void Block() { std::this_thread::sleep_for(std::chrono::hours(24)); }

  std::shared_ptr<Router> GetRouter() {
    std::cout << "getting a router" << std::endl;
    return std::make_shared<Router>();
  }

  std::string DumpRoutingTable();

  void HandleRequest(express_request_t req, express_response_t res);

 private:
  template <typename HandlerType>
  void RegisterPath(std::string path, HttpMethod method, HandlerType handler) {
    std::cout << "handler registered for expresscpp path " << path
              << " and method " << magic_enum::enum_name(method) << std::endl;
    ExpressCppHandler express_cpp_handler;
    express_cpp_handler.method = method;
    express_cpp_handler.debug_function_name = typeid(handler).name();

    express_cpp_handler.handler = handler;
    handler_map_[path].push(express_cpp_handler);
    // TODO: handle path = "*" -> always call this handler e.g. logger
  }

  std::vector<Router> routers;

  std::shared_ptr<Listener> listener_{nullptr};

  std::map<std::string, express_handler_queue_t> handler_map_;

  // The io_context is required for all I/O
  boost::asio::io_context ioc{threads};
  std::vector<std::thread> io_threads;
};
