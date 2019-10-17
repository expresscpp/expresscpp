#pragma once

#include <chrono>
#include <map>
#include <string>

#include "boost/beast/http/verb.hpp"
#include "boost/uuid/uuid.hpp"

#include "expresscpp/http_method.hpp"

class Request {
 public:
  Request();
  Request(std::string_view path, HttpMethod method);

  std::map<std::string, std::string> headers_;
  std::string_view path_;
  HttpMethod method_;
  boost::uuids::uuid uuid_;
  std::chrono::system_clock::time_point timestamp_;
  int version_;
};
