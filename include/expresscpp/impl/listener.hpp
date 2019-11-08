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
#include "expresscpp/types.hpp"

namespace expresscpp {

class ExpressCpp;

//! @brief Accepts incoming connections and launches the sessions
class Listener : public std::enable_shared_from_this<Listener> {
  ExpressCpp* express_cpp_;

 public:
  Listener(boost::asio::ip::tcp::endpoint endpoint, ExpressCpp* express_cpp, ready_fn_cb_error_code_t error_callback);

  ~Listener() {
    Console::Debug("destroying listener");
    Stop();
  }

  //! @brief Start accepting incoming connections
  void run();

  void launch_threads() {
    // Run the I/O service on the requested number of threads
    io_threads.reserve(threads_);
    for (auto i = threads_; i > 0; --i) {
      io_threads.emplace_back([this] { ioc_.run(); });
    }
    listening_ = true;
  }

  void Stop() {
    if (!listening_) {
      return;
    }
    ioc_.stop();
    acceptor_.close();
    for (auto& t : io_threads) {
      if (t.joinable()) {
        t.join();
      }
    }
    Console::Debug("stopping listener");
    listening_ = false;
  }

  std::size_t threads_{4u};

  //! @brief the io_context is required for all I/O
  boost::asio::io_context ioc_;

  std::vector<std::thread> io_threads;

  boost::asio::ip::tcp::acceptor acceptor_;

 private:
  void do_accept();

  void on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);

  bool listening_{false};
};
}  // namespace expresscpp
