#pragma once

#include <algorithm>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/filesystem.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "boost/config.hpp"
#include "expresscpp/impl/utils.hpp"

namespace expresscpp {

class ExpressCpp;
class Response;
class Listener;

using tcp = boost::asio::ip::tcp;  // from <boost/asio.hpp>
using SessionParser = http::request_parser<http::string_body>;

// Handles an HTTP server connection
class Session : public std::enable_shared_from_this<Session> {
  friend Response;

 public:
  // Take ownership of the stream
  Session(tcp::socket socket, ExpressCpp* express_cpp);

  ~Session();

  tcp::socket socket_;

  beast::flat_buffer buffer_{8192};
  std::shared_ptr<void> res_;
  ExpressCpp* express_cpp_;

  // Start the asynchronous operation
  void run();

 private:
  void do_read();

  void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);

  void do_close();

  boost::asio::strand<boost::asio::io_context::executor_type> strand_;

  // The parser for reading the requests
  std::unique_ptr<SessionParser> parser_;

 protected:
  void on_write(bool close, boost::beast::error_code ec, std::size_t bytes_transferred);
};
}  // namespace expresscpp
