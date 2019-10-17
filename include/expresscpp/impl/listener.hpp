#pragma once

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

#include "expresscpp/impl/session.hpp"
#include "expresscpp/impl/utils.hpp"

namespace beast = boost::beast;    // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>
namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

class ExpressCpp;

// Accepts incoming connections and launches the sessions
class Listener : public std::enable_shared_from_this<Listener> {
  net::io_context& ioc_;
  tcp::acceptor acceptor_;
  ExpressCpp* express_cpp_;

 public:
  Listener(net::io_context& ioc, tcp::endpoint endpoint, ExpressCpp* express_cpp);

  // Start accepting incoming connections
  void run();

 private:
  void do_accept();

  void on_accept(beast::error_code ec, tcp::socket socket);
};
