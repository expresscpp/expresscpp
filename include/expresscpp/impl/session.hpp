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
  // TODO(gocarlos): improve this
  // This is the C++11 equivalent of a generic lambda.
  // The function object is used to send an HTTP message.
  struct send_lambda {
    Session& self_;

    explicit send_lambda(Session& self) : self_(self) {
    }

    template <bool isRequest, class Body, class Fields>
    void operator()(boost::beast::http::message<isRequest, Body, Fields>&& msg) const {
      // The lifetime of the message has to extend
      // for the duration of the async operation so
      // we use a shared_ptr to manage it.
      auto sp = std::make_shared<boost::beast::http::message<isRequest, Body, Fields>>(std::move(msg));

      // Store a type-erased version of the shared
      // pointer in the class to keep it alive.
      self_.res_ = sp;

      // Write the response
      boost::beast::http::async_write(
          self_.stream_, *sp,
          boost::beast::bind_front_handler(&Session::on_write, self_.shared_from_this(), sp->need_eof()));
    }
  };

 public:
  boost::beast::tcp_stream stream_;
  boost::beast::flat_buffer buffer_;
  boost::beast::http::request<boost::beast::http::string_body> req_;
  std::shared_ptr<void> res_;
  send_lambda lambda_;
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
