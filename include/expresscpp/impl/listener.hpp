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
namespace expresscpp {

class ExpressCpp;

//! @brief Accepts incoming connections and launches the sessions
class Listener : public std::enable_shared_from_this<Listener> {
  boost::asio::io_context& ioc_;
  boost::asio::ip::tcp::acceptor acceptor_;
  ExpressCpp* express_cpp_;

 public:
  Listener(boost::asio::io_context& ioc, boost::asio::ip::tcp::endpoint endpoint, ExpressCpp* express_cpp);

  //! @brief Start accepting incoming connections
  void run();

 private:
  void do_accept();

  void on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);
};
}  // namespace expresscpp
