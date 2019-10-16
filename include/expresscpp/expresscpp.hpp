#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "boost/asio.hpp"

#include "expresscpp/impl/listener.hpp"
#include "expresscpp/impl/session.hpp"
#include "expresscpp/nextrouter.hpp"
#include "expresscpp/request.hpp"
#include "expresscpp/response.hpp"
#include "expresscpp/router.hpp"
#include "expresscpp/types.hpp"

auto const threads = std::max<int>(1, std::atoi("4"));
const auto address = boost::asio::ip::make_address("0.0.0.0");

class ExpressCpp {
 public:
  ExpressCpp();
  ~ExpressCpp();

  void HandleRequest(std::shared_ptr<Request> req,
                     std::shared_ptr<Response> res);

  //  template <typename HandlerType>
  //  void Get(std::string path, HandlerType handler) {
  //    std::cout << "registering get for path: " << path << std::endl;
  //  }

  void Get(std::string path, express_handler_t handler) {
    std::cout << "registering get for path: " << path << std::endl;

    ExpressCppHandler express_cpp_handler;
    express_cpp_handler.method = HttpMethod::Get;
    express_cpp_handler.debug_function_name = typeid(handler).name();
    express_cpp_handler.handler = handler;
    handler_map_[path].push(express_cpp_handler);
  }

  template <typename Callback>
  void Listen(uint16_t port, Callback callback) {
    // Create and launch a listening port
    std::make_shared<Listener>(ioc, tcp::endpoint{address, port}, this)->run();

    // Run the I/O service on the requested number of threads
    io_threads.reserve(threads);
    for (auto i = threads; i > 0; --i) {
      io_threads.emplace_back([this] { ioc.run(); });
    }
    callback();
  }

  std::shared_ptr<Router> GetRouter() {
    std::cout << "getting a router" << std::endl;
    return std::make_shared<Router>();
  }

  template <typename HandlerType>
  void Use(std::string path, HandlerType handler) {
    std::cout << "handler registered for expresscpp path " << path << std::endl;
    ExpressCppHandler express_cpp_handler;
    express_cpp_handler.method = HttpMethod::All;
    express_cpp_handler.debug_function_name = typeid(handler).name();

    express_cpp_handler.handler = handler;
    handler_map_[path].push(express_cpp_handler);
    // TODO: handle path = "*" -> always call this handler e.g. logger
  }

  std::string DumpRoutingTable();

 private:
  std::vector<Router> routers;

  std::map<std::string, express_handler_queue_t> handler_map_;

  // The io_context is required for all I/O
  boost::asio::io_context ioc{threads};
  std::vector<std::thread> io_threads;
};
