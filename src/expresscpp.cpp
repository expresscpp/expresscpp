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

#include "boost/algorithm/string.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/strand.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "boost/config.hpp"
#include "expresscpp/handler.hpp"
#include "expresscpp/impl/listener.hpp"
#include "expresscpp/impl/routing_stack.hpp"
#include "expresscpp/impl/session.hpp"
#include "expresscpp/route.hpp"
#include "nlohmann/json.hpp"

namespace expresscpp {

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

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
  RegisterPath(registered_path, HttpMethod::Get, handler);
}

void ExpressCpp::Post(std::string_view registered_path, express_handler_wn_t handler) {
  assert(handler != nullptr);
  RegisterPath(registered_path, HttpMethod::Post, handler);
}

void ExpressCpp::Delete(std::string_view registered_path, express_handler_wn_t handler) {
  assert(handler != nullptr);
  RegisterPath(registered_path, HttpMethod::Delete, handler);
}

void ExpressCpp::Patch(std::string_view registered_path, express_handler_wn_t handler) {
  assert(handler != nullptr);
  RegisterPath(registered_path, HttpMethod::Patch, handler);
}

std::shared_ptr<Route> ExpressCpp::CreateRoute(const std::string_view registered_path) {
  lazyrouter();
  return _router->CreateRoute(registered_path);
}

void ExpressCpp::Use(express_handler_t handler) {
  Console::Debug("using handler for all paths");
  RegisterPath("/", HttpMethod::All, handler);
}

void ExpressCpp::Use(express_handler_wn_t handler) {
  Console::Debug("using handler for all paths");
  lazyrouter();
  _router->Use("/", handler);
}

void ExpressCpp::Use(std::string_view registered_path, express_handler_t handler) {
  RegisterPath(registered_path, HttpMethod::All, handler);
}
void ExpressCpp::Use(std::string_view registered_path, express_handler_wn_t handler) {
  RegisterPath(registered_path, HttpMethod::All, handler);
}

void ExpressCpp::Use(std::string_view registered_path, RouterPtr router) {
  //  routers_.push_back({registered_path, router});
  Console::Debug(fmt::format(R"(adding router "{}" to path "{}")", router->GetName(), registered_path));
  RegisterPath(
      registered_path, HttpMethod::All, [&](auto req, auto res) { router->HandleRequest(req, res); }, true);
}

ExpressCpp& ExpressCpp::Listen(uint16_t port, ready_fn_cb_error_code_t callback) {
  std::error_code ec;
  if (listening_) {
    Console::Error("already listening");
    ec = std::make_error_code(std::errc::already_connected);
    callback(ec);
    return *this;
  }

  setPort(port);
  const auto address = boost::asio::ip::make_address("0.0.0.0");

  // Create and launch a listening port
  listener_ = std::make_shared<Listener>(boost::asio::ip::tcp::endpoint{address, port_}, this, [&](auto listen_ec) {
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
  RegisterPath("/", HttpMethod::Get, [&](auto req, auto res) { static_file_provider->HandleRequests(req, res); });
}

void ExpressCpp::Use(std::string_view path, StaticFileProviderPtr static_file_provider) {
  RegisterPath(path, HttpMethod::Get, [&](auto req, auto res) {
    static_file_provider->UsePrefix(path);
    static_file_provider->HandleRequests(req, res);
  });
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

  if (callback == nullptr) {
    callback = []() {
      // TODO(gocarlos): print here the callstack...
      Console::Error("ERROR");
    };
  }

  if (_router == nullptr) {
    Console::Error("no routes defined on app");
    callback();
    return;
  }

  Console::Debug(fmt::format(R"(handling request: "{}" "{}")", getHttpMethodName(req->getMethod()), req->getPath()));

  _router->HandleRequest(req, res);
}

static int stack_print_intendation = 0;

std::vector<RoutingStack> ExpressCpp::Stack() const {
  std::vector<RoutingStack> routing_stack;

  for (const auto& l : _router->stack()) {
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

void ExpressCpp::RegisterPath(const std::string_view registered_path, const HttpMethod method,
                              express_handler_t /*handler*/, const bool is_router) {
  if (is_router) {
    Console::Debug(
        fmt::format(R"(subrouter registered path "{}", method "{}")", registered_path, getHttpMethodName(method)));
  } else {
    Console::Debug(
        fmt::format(R"(handler registered path "{}", method "{}")", registered_path, getHttpMethodName(method)));
  }
  //  routers_[0].second->routes_.push_back({path, method});

  //  ExpressCppHandler express_cpp_handler;
  //  express_cpp_handler.setMethod(method);
  //  express_cpp_handler.setIs_router(is_router);
  //  express_cpp_handler.setDebug_function_name(typeid(handler).name());

  //  express_cpp_handler.handler = handler;
  //  handler_map_[path].push_back(express_cpp_handler);
  // TODO(gocarlos): handle path = "*" -> always call this handler e.g. logger
}

void ExpressCpp::RegisterPath(const std::string_view registered_path, const HttpMethod method,
                              express_handler_wn_t handler) {
  lazyrouter();

  auto route = _router->CreateRoute(registered_path);
  Console::Debug(fmt::format("registering path \"{}\"", registered_path));

  PathToRegExpOptions op;
  auto layer = std::make_shared<Layer>("/", op, handler);
  layer->setMethod(method);
  route->methods_.insert(method);
  route->stack_.push_back(layer);
}

void ExpressCpp::Init() {
  Console::Debug("ExpressCpp created");
  //  auto r = std::make_shared<Router>("base router");
  //  routers_.push_back({"", r});

  finished = false;
  InstallSignalHandler();
}

void ExpressCpp::lazyrouter() {
  if (_router == nullptr) {
    _router = std::make_shared<Router>("base router");
  }
}

std::uint16_t ExpressCpp::port() const {
  return port_;
}

void ExpressCpp::setPort(const std::uint16_t& port) {
  port_ = port;
}

std::string ExpressCpp::DumpRoutingTable() const {
  nlohmann::json json_object = nlohmann::json::object();
  nlohmann::json json_routes = nlohmann::json::array();
  for (const auto& h : handler_map_) {
    nlohmann::json json_route = nlohmann::json::object();
    json_route["path"] = h.first;
    json_route["number_of_handlers"] = std::to_string(h.second.size());

    auto tmp_vec = h.second;  // copy the original queue to the temporary queue
    nlohmann::json json_handlers = nlohmann::json::array();

    while (!tmp_vec.empty()) {
      auto q_element = tmp_vec.front();
      nlohmann::json json = nlohmann::json::object();

      const auto method_name = getHttpMethodName(q_element.getMethod());
      json["method"] = method_name;
      json["addr"] = q_element.getDebug_function_name();
      json_handlers.push_back(json);
      tmp_vec.pop_back();
    }
    json_route["handlers"] = json_handlers;
    json_routes.push_back(json_route);
  }

  json_object["routes"] = json_routes;
  const std::string routing_table = json_object.dump(4);
  return routing_table;
}

void printRouters(const std::pair<std::string_view, std::shared_ptr<Router>> /*r*/,
                  const std::vector<std::pair<std::string_view, std::shared_ptr<Router>>> /*routers*/) {
  stack_print_intendation++;

  //  // loop through all routes for this router
  //  for (const auto& v : r.second->routes_) {
  //    std::string a = "";
  //    for (int i = 0; i < stack_print_intendation * 4; i++) {
  //      a += " ";
  //    }

  //    // print this route for this router
  //    std::cout << a << v.getMethodName() << ": \"" << v.getPath() << "\"" << std::endl;

  //    for (const auto& rs : routers) {
  //      if (rs.first != "" && rs.first == v.getPath()) {
  //        //        std::cout << "subpath is " << rs.first << std::endl;
  //        printRouters(rs, r.second->GetRouters());
  //      }
  //    }
  //  }
  //  if (r.second->GetRouters().size() > 0) {
  //    //    std::cout << "router has subrouters" << std::endl;
  //    for (const auto& subrouter : r.second->GetRouters()) {
  //      printRouters(subrouter, r.second->GetRouters());
  //    }
  //  }

  stack_print_intendation--;
}
}  // namespace expresscpp
