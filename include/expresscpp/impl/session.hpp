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
#include "expresscpp/impl/utils.hpp"

namespace expresscpp {

namespace beast = boost::beast;
namespace http = beast::http;

class ExpressCpp;

// Handles an HTTP server connection
class Session : public std::enable_shared_from_this<Session> {
 public:
  boost::beast::tcp_stream stream_;
  boost::beast::flat_buffer buffer_;
  boost::beast::http::request<boost::beast::http::string_body> req_;
  std::shared_ptr<void> res_;
  ExpressCpp* express_cpp_;

  // Take ownership of the stream
  Session(tcp::socket&& socket, ExpressCpp* express_cpp);

  // Start the asynchronous operation
  void run();

  void do_read();

  void on_read(beast::error_code ec, std::size_t bytes_transferred);

  void on_write(bool close, beast::error_code ec, std::size_t bytes_transferred);

  void do_close();
};
}  // namespace expresscpp
