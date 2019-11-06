#pragma once

#include <map>
#include <string>

#include "boost/beast/http/verb.hpp"
#include "expresscpp/http_method.hpp"

namespace expresscpp {

struct FetchOptions {
  HttpMethod method = HttpMethod::Unknown;
  std::map<std::string, std::string> headers;
  std::string body;
};

std::string fetch(const std::string& url, FetchOptions);

std::string fetch(const std::string& path, boost::beast::http::verb verb = boost::beast::http::verb::get,
                  std::map<std::string, std::string> headers = {});

}  // namespace expresscpp
