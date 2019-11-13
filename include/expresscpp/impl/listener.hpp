#pragma once

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <set>
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
  Listener(const std::string& address, const uint16_t port, ExpressCpp* express_cpp,
           ready_fn_cb_error_code_t error_callback);

  ~Listener();

  //! @brief Start accepting incoming connections
  void run();

  void launch_threads();

  void Stop();

  std::size_t threads_{4u};

  //! @brief the io_context is required for all I/O
  boost::asio::io_context ioc_;

  std::vector<std::thread> io_threads;

  boost::asio::ip::tcp::acceptor acceptor_;

 private:
  void Init(boost::asio::ip::tcp::endpoint endpoint, ready_fn_cb_error_code_t error_callback);

  void do_accept();

  void on_accept(boost::beast::error_code ec);

  std::mutex mutex_;

  bool listening_{false};

  boost::asio::strand<boost::asio::io_context::executor_type> strand_;

  boost::asio::ip::tcp::socket socket_;
};
}  // namespace expresscpp
