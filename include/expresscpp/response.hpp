#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "boost/beast/http/message.hpp"
#include "boost/beast/http/string_body.hpp"
#include "boost/uuid/uuid.hpp"

namespace expresscpp {

class Session;

class Response {
 public:
  explicit Response(Session* session);

  void SetStatus(uint16_t status);

  void KeepAlive(bool alive);

  void Send(std::string message);

  void Json(std::string_view json_string);

  const std::map<std::string, std::string>& GetParams() const;

  const std::map<std::string, std::string>& GetQueryParams() const;

  const std::string& GetQueryString() const;

  void SetQueryString(const std::string& query_string);

  void SetParams(const std::map<std::string, std::string>& params);

  void SetQueryParams(const std::map<std::string, std::string>& query_params);

 public:
  boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::ok, 11};
  void SendInternal();

  Session* session_{nullptr};
  bool response_sent_{false};

 private:
  uint16_t status_{200u};
  boost::uuids::uuid uuid_;
  std::map<std::string, std::string> params_;
  std::map<std::string, std::string> query_params_;
  std::string query_string_;
};

typedef std::shared_ptr<Response> express_response_t;

}  // namespace expresscpp
