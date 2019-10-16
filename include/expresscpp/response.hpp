#pragma once

#include <string>
#include <string_view>

#include "boost/beast/http/message.hpp"
#include "boost/beast/http/string_body.hpp"

class Session;

class Response {
 public:
  explicit Response(Session* session) : session_(session) {}

  void SetStatus(uint16_t status) { status_ = status; }

  void Send(std::string message);

  void Json(std::string_view json_string);

 private:
  void SendInternal(
      boost::beast::http::response<boost::beast::http::string_body> res);

  uint16_t status_;
  Session* session_;

};
