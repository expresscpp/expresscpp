#include "expresscpp/expresscpp.hpp"

#include <algorithm>
#include <condition_variable>
#include <csignal>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "expresscpp/handler.hpp"
#include "expresscpp/impl/listener.hpp"
#include "expresscpp/impl/routing_stack.hpp"
#include "expresscpp/impl/session.hpp"
#include "expresscpp/route.hpp"
#include "nlohmann/json.hpp"

namespace expresscpp {

//! used when the user whats the listen to block until CTRL+C
static std::mutex running_mtx;
static std::condition_variable running_cv;
static bool finished = false;

ExpressCpp::ExpressCpp() {
  Init();
}

ExpressCpp::~ExpressCpp() {
  if (listener_) {
    listener_->Stop();
  }
  Console::Debug("ExpressCpp destroyed");
}

void ExpressCpp::Get(std::string_view registered_path, express_handler_wn_t handler) {
  assert(handler != nullptr);
  router_->Get(registered_path, handler);
}

void ExpressCpp::Post(std::string_view registered_path, express_handler_wn_t handler) {
  assert(handler != nullptr);
  router_->Post(registered_path, handler);
}

void ExpressCpp::Delete(std::string_view registered_path, express_handler_wn_t handler) {
  assert(handler != nullptr);
  router_->Delete(registered_path, handler);
}

void ExpressCpp::Patch(std::string_view registered_path, express_handler_wn_t handler) {
  assert(handler != nullptr);
  router_->Patch(registered_path, handler);
}

void ExpressCpp::Error(express_handler_wecn_t handler) {
  (void)handler;
  error_handler_ = handler;
  Console::Debug("error handler registered");
  error_handler_registered_ = true;
}

std::shared_ptr<Route> ExpressCpp::CreateRoute(const std::string_view registered_path) {
  return router_->CreateRoute(registered_path);
}

void ExpressCpp::Use(express_handler_t handler) {
  Console::Debug("using handler for all paths");
  //  RegisterPath("/", HttpMethod::All, handler);
  throw std::runtime_error("not implemented yet");
}

void ExpressCpp::Use(express_handler_wn_t handler) {
  Console::Debug("using handler for all paths");
  router_->Use("/", handler);
}

void ExpressCpp::Use(std::string_view registered_path, express_handler_t handler) {
  //  RegisterPath(registered_path, HttpMethod::All, handler);
  throw std::runtime_error("not implemented yet");
}

void ExpressCpp::Use(std::string_view registered_path, express_handler_wn_t handler) {
  router_->RegisterPath(registered_path, HttpMethod::All, handler);
}

void ExpressCpp::Use(std::string_view registered_path, RouterPtr router) {
  Console::Debug(fmt::format(R"(adding router "{}" to path "{}")", router->GetName(), registered_path));
  router_->Use(registered_path, router);
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

#ifdef EXPRESSCPP_ENABLE_STATIC_FILE_PROVIDER
void ExpressCpp::Use(StaticFileProviderPtr static_file_provider) {
  router_->RegisterPath("/", HttpMethod::Get,
                        [&](auto req, auto res, auto /*next*/) { static_file_provider->HandleRequests(req, res); });
}

void ExpressCpp::Use(std::string_view path, StaticFileProviderPtr static_file_provider) {
  throw std::runtime_error("not implemented yet");
  //  RegisterPath(path, HttpMethod::Get, [&](auto req, auto res) {
  //    static_file_provider->UsePrefix(path);
  //    static_file_provider->HandleRequests(req, res);
  //  });
}
#endif

void ExpressCpp::Block() {
  std::unique_lock<std::mutex> lck(running_mtx);

  if (!finished) {
    running_cv.wait(lck);
  }
}

void ExpressCpp::InstallSignalHandler() {
  std::signal(SIGINT, ExpressCpp::HandleSignal);
}

void ExpressCpp::HandleSignal(int signal) {
  // print new line to get rid of the ctrl+c in the terminal
  std::cout << std::endl;
  Console::Log(fmt::format("Got Signal: {}", signal));
  finished = true;
  running_cv.notify_all();
}

RouterPtr ExpressCpp::GetRouter() {
  Console::Debug("getting a router");
  auto r = std::make_shared<Router>();
  return r;
}

RouterPtr ExpressCpp::GetRouter(std::string_view name) {
  Console::Debug(fmt::format("getting a router{}", name));
  auto r = std::make_shared<Router>(name);
  return r;
}

#ifdef EXPRESSCPP_ENABLE_STATIC_FILE_PROVIDER
StaticFileProviderPtr ExpressCpp::GetStaticFileProvider(const std::filesystem::path& path_to_root_folder) {
  if (!std::filesystem::exists(path_to_root_folder)) {
    throw std::runtime_error("path to root folder with static files does not exist");
  }

  auto p = std::make_shared<StaticFileProvider>(path_to_root_folder);
  static_file_providers_.push_back(p);
  return p;
}
#endif

void ExpressCpp::HandleRequest(express_request_t req, express_response_t res, std::function<void()> callback) {
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
      auto ec = std::make_error_code(std::errc::no_message);
      error_handler_(ec, req, res, nullptr);
    } else {
      std::rethrow_exception(eptr);
    }
  }
}

void ExpressCpp::HandleRequest(std::error_code ec, express_request_t req, express_response_t res,
                               std::function<void(const std::error_code)> callback) {
  (void)ec;

  assert(req != nullptr);
  assert(res != nullptr);
  if (callback == nullptr) {
    callback = [](const auto ec) {
      // TODO(gocarlos): print here the callstack...
      Console::Error(fmt::format("ERROR: {}", ec.message()));
    };
  }
}

std::vector<RoutingStack> ExpressCpp::Stack() const {
  std::vector<RoutingStack> routing_stack;

  for (const auto& l : router_->stack()) {
    // if route is nullptr -> this means it is a middleware
    if (l->getRoute() != nullptr) {
      for (const auto& ll : l->getRoute()->stack_) {
        Console::Debug(
            fmt::format(R"(registered paths: "{}" "{}")", l->getRoute()->getPath(), getHttpMethodName(ll->method())));

        RoutingStack rs{.path = l->getRoute()->getPath().data(), .method = ll->method()};
        routing_stack.push_back(rs);
      }
    } else {
      RoutingStack rs{.path = l->path_, .method = l->method()};
      routing_stack.push_back(rs);
    }
  }
  return routing_stack;
}

void ExpressCpp::Init() {
  Console::Debug("ExpressCpp created");
  router_ = std::make_unique<Router>("base router");
  finished = false;
  InstallSignalHandler();
}

}  // namespace expresscpp
