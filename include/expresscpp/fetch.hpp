#pragma once

#include <map>
#include <string>

#include "expresscpp/http_method.hpp"

namespace expresscpp {

struct FetchOptions {
  HttpMethod method = HttpMethod::Get;
  std::map<std::string, std::string> headers;
  std::string body;
};

std::string fetch(const std::string& url, FetchOptions = {});

}  // namespace expresscpp
