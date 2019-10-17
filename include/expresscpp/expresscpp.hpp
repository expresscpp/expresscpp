#pragma once

#include <filesystem>
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

#include "expresscpp/middleware/staticfileprovider.hpp"

const auto threads = std::max<int>(1, std::atoi("4"));

class ExpressCpp {
 public:
  ExpressCpp();
  ~ExpressCpp();

  void Get(std::string path, express_handler_t handler) { RegisterPath(path, HttpMethod::Get, handler); }
  void Post(std::string path, express_handler_t handler) { RegisterPath(path, HttpMethod::Post, handler); }
  void Delete(std::string path, express_handler_t handler) { RegisterPath(path, HttpMethod::Delete, handler); }
  void Patch(std::string path, express_handler_t handler) { RegisterPath(path, HttpMethod::Patch, handler); }
  void Use(std::string path, express_handler_t handler) { RegisterPath(path, HttpMethod::All, handler); }
  void Use(StaticFileProviderPtr static_file_provider) {
    RegisterPath("/", HttpMethod::Get, [&](auto req, auto res) { static_file_provider->HandleRequests(req, res); });
  }
  void Use(std::string path, StaticFileProviderPtr static_file_provider) {
    RegisterPath(path, HttpMethod::Get, [&](auto req, auto res) {
      static_file_provider->UsePrefix(path);
      static_file_provider->HandleRequests(req, res);
    });
  }

  template <typename Callback>
  ExpressCpp& Listen(uint16_t port, Callback callback) {
    const auto address = boost::asio::ip::make_address("0.0.0.0");

    // Create and launch a listening port
    listener_ = std::make_shared<Listener>(ioc, boost::asio::ip::tcp::endpoint{address, port}, this);
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

  std::shared_ptr<Router> GetRouter();

  StaticFileProviderPtr GetStaticFileProvider(const std::filesystem::path& path_to_root_folder);

  std::string DumpRoutingTable();

  void HandleRequest(express_request_t req, express_response_t res);

 private:
  void RegisterPath(std::string path, HttpMethod method, express_handler_t handler);

  std::vector<Router> routers;

  std::vector<StaticFileProviderPtr> static_file_providers_;

  std::shared_ptr<Listener> listener_{nullptr};

  std::map<std::string, express_handler_queue_t> handler_map_;

  // The io_context is required for all I/O
  boost::asio::io_context ioc{threads};
  std::vector<std::thread> io_threads;
};
