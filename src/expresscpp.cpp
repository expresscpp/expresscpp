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

#include "expresscpp/impl/listener.hpp"
#include "expresscpp/impl/session.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

ExpressCpp::ExpressCpp() { std::cout << "ExpressCpp created" << std::endl; }

ExpressCpp::~ExpressCpp() {
  std::cout << "ExpressCpp destroyed" << std::endl;

  ioc.stop();
  for (auto &t : io_threads) {
    if (t.joinable()) {
      t.join();
    }
  }
}

void ExpressCpp::HandleRequest(std::shared_ptr<Request> req,
                               std::shared_ptr<Response> res) {
  assert(req != nullptr);
  assert(res != nullptr);
  std::cout << "handling request for path: " << req->path_ << std::endl;

  res->Json(R"({"status":"ok"})");
  //  res->Send("asdfadsf");
  //    for (auto &r : routers) {
  //      r.HandleRequest(req, res);
  //    }
}
