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
#ifndef NDEBUG
  Use("/debug", [this](auto /*req*/, auto res) {
    const auto dump = DumpRoutingTable();
    res->Json(dump);
  });
#endif
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

void ExpressCpp::HandleRequest(express_request_t req, express_response_t res) {
  assert(req != nullptr);
  assert(res != nullptr);

  std::cout << "handling request for path: \"" << req->path_
            << "\" and method \"" << magic_enum::enum_name(req->method_) << "\""
            << std::endl;

  for (const auto& h : handler_map_) {
    std::cout << "trying path " << h.first << std::endl;

    if (h.second.front().method == HttpMethod::All ||
        req->method_ == h.second.front().method) {
      if (h.first == req->path_) {
        std::cout << "path is registered" << std::endl;
        auto handler = h.second.front().handler;
        handler(req, res);
        return;
      }
    }
  }
  res->Json(R"({"status":"error"})");
  //  res->Send("asdfadsf");
  //    for (auto &r : routers) {
  //      r.HandleRequest(req, res);
  //    }
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

      const auto color_name = magic_enum::enum_name(q_element.method);
      json["method"] = color_name;
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
