#pragma once

#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "boost/asio.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "magic_enum.hpp"

#include "expresscpp/impl/listener.hpp"
#include "expresscpp/impl/session.hpp"
#include "expresscpp/nextrouter.hpp"
#include "expresscpp/request.hpp"
#include "expresscpp/response.hpp"
#include "expresscpp/router.hpp"
#include "expresscpp/types.hpp"

#include "expresscpp/middleware/staticfileprovider.hpp"

const std::size_t threads = 4u;

void printRoutes(std::pair<std::string_view, std::shared_ptr<Router>> r,
                 std::vector<std::pair<std::string_view, std::shared_ptr<Router>>> routers);

void DumpStack(std::vector<std::pair<std::string_view, std::shared_ptr<Router>>> routers);

void PrintBaseRoutes(std::vector<std::pair<std::string_view, std::shared_ptr<Router>>> routers);

class ExpressCpp {
 public:
  ExpressCpp();
  ~ExpressCpp();

  // http verbs
  void Get(std::string_view path, express_handler_t handler) {
    RegisterPath(path, HttpMethod::Get, handler);
  }
  void Post(std::string_view path, express_handler_t handler) {
    RegisterPath(path, HttpMethod::Post, handler);
  }
  void Delete(std::string_view path, express_handler_t handler) {
    RegisterPath(path, HttpMethod::Delete, handler);
  }
  void Patch(std::string_view path, express_handler_t handler) {
    RegisterPath(path, HttpMethod::Patch, handler);
  }

  void Use(express_handler_t handler) { RegisterPath("", HttpMethod::All, handler); }

  void Use(std::string_view path, express_handler_t handler) {
    RegisterPath(path, HttpMethod::All, handler);
  }
  void Use(std::string_view path, RouterPtr router) {
    std::cout << "adding router to path: " << path << std::endl;
    routers.push_back({path, router});
    RegisterPath(path, HttpMethod::All,
                 [&](auto req, auto res) { router->HandleRequest(req, res); });
  }

  void Use(StaticFileProviderPtr static_file_provider) {
    RegisterPath("/", HttpMethod::Get,
                 [&](auto req, auto res) { static_file_provider->HandleRequests(req, res); });
  }
  void Use(std::string_view path, StaticFileProviderPtr static_file_provider) {
    RegisterPath(path, HttpMethod::Get, [&](auto req, auto res) {
      static_file_provider->UsePrefix(path);
      static_file_provider->HandleRequests(req, res);
    });
  }

  template <typename Callback>
  ExpressCpp& Listen(uint16_t port, Callback callback) {
    const auto address = boost::asio::ip::make_address("0.0.0.0");

    // Create and launch a listening port
    listener_ =
        std::make_shared<Listener>(ioc, boost::asio::ip::tcp::endpoint{address, port}, this);
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

  RouterPtr GetRouter();
  RouterPtr GetRouter(std::string_view name);

  StaticFileProviderPtr GetStaticFileProvider(const std::filesystem::path& path_to_root_folder);

  std::string DumpRoutingTable();

  void HandleRequest(express_request_t req, express_response_t res);

  void Stack() const;

 private:
  void RegisterPath(std::string_view path, HttpMethod method, express_handler_t handler);

  std::vector<std::pair<std::string_view, std::shared_ptr<Router>>> routers;

  std::vector<StaticFileProviderPtr> static_file_providers_;

  std::shared_ptr<Listener> listener_{nullptr};

  std::map<std::string_view, express_handler_queue_t> handler_map_;

  // The io_context is required for all I/O
  boost::asio::io_context ioc{threads};
  std::vector<std::thread> io_threads;
};
