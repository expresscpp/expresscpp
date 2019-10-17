#include "expresscpp/expresscpp.hpp"

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/strand.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "boost/config.hpp"
#include "magic_enum.hpp"
#include "nlohmann/json.hpp"

#include "expresscpp/impl/listener.hpp"
#include "expresscpp/impl/session.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

ExpressCpp::ExpressCpp() {
  std::cout << "ExpressCpp created" << std::endl;
  //#ifndef NDEBUG
  //  Get("/debug", [this](auto /*req*/, auto res) {
  //    const auto dump = DumpRoutingTable();
  //    res->Json(dump);
  //  });
  //#endif
}

ExpressCpp::~ExpressCpp() {
  std::cout << "ExpressCpp destroyed" << std::endl;

  ioc.stop();
  for (auto& t : io_threads) {
    if (t.joinable()) {
      t.join();
    }
  }
}

std::shared_ptr<Router> ExpressCpp::GetRouter() {
  std::cout << "getting a router" << std::endl;
  return std::make_shared<Router>();
}

StaticFileProviderPtr ExpressCpp::GetStaticFileProvider(const std::filesystem::path& path_to_root_folder) {
  if (!std::filesystem::exists(path_to_root_folder)) {
    throw std::runtime_error("path to root folder with static files does not exist");
  }

  auto p = std::make_shared<StaticFileProvider>(path_to_root_folder);
  static_file_providers_.push_back(p);
  return p;
}

void ExpressCpp::HandleRequest(express_request_t req, express_response_t res) {
  assert(req != nullptr);
  assert(res != nullptr);

  std::cout << "handling request for path: \"" << req->path_ << "\" and method \""
            << magic_enum::enum_name(req->method_) << "\"" << std::endl;

  for (const auto& h : handler_map_) {
    std::cout << "trying path " << h.first << std::endl;

    const std::string requested_path = req->path_.data();
    const std::string registered_path = h.first;
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

void ExpressCpp::RegisterPath(std::string path, HttpMethod method, express_handler_t handler) {
  std::cout << "handler registered for expresscpp path " << path << " and method " << magic_enum::enum_name(method)
            << std::endl;
  ExpressCppHandler express_cpp_handler;
  express_cpp_handler.method = method;
  express_cpp_handler.debug_function_name = typeid(handler).name();

  express_cpp_handler.handler = handler;
  handler_map_[path].push(express_cpp_handler);
  // TODO: handle path = "*" -> always call this handler e.g. logger
}

std::string ExpressCpp::DumpRoutingTable() {
  nlohmann::json json_object = nlohmann::json::object();
  nlohmann::json json_routes = nlohmann::json::array();
  for (const auto& h : handler_map_) {
    nlohmann::json json_route = nlohmann::json::object();
    json_route["path"] = h.first;
    json_route["number_of_handlers"] = std::to_string(h.second.size());

    auto tmp_q = h.second;  // copy the original queue to the temporary queue
    nlohmann::json json_handlers = nlohmann::json::array();

    while (!tmp_q.empty()) {
      auto q_element = tmp_q.front();
      nlohmann::json json = nlohmann::json::object();

      const auto method_name = magic_enum::enum_name(q_element.method);
      json["method"] = method_name;
      json["addr"] = q_element.debug_function_name;
      json_handlers.push_back(json);
      tmp_q.pop();
    }
    json_route["handlers"] = json_handlers;
    json_routes.push_back(json_route);
  }

  json_object["routes"] = json_routes;
  const std::string routing_table = json_object.dump(4);
  return routing_table;
}
