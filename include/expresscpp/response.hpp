#pragma once

#include <algorithm>
#include <cstdint>
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

namespace beast = boost::beast;  
namespace http = beast::http;    
namespace net = boost::asio;     
using tcp = boost::asio::ip::tcp;

class session;

class Response {
 public:
  explicit Response(session* session) : session_(session) {}

  void SetStatus(uint16_t status) { status_ = status; }

  void Send(std::string message);

  void Json(std::string_view json_string);

 private:
  void SendInternal(http::response<http::string_body> res);

  uint16_t status_;
  session* session_;

};
