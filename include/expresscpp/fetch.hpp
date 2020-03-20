#pragma once

#include <map>
#include <string>

#include "expresscpp/http_method.hpp"
#include "expresscpp/exports.hpp"

namespace expresscpp {

struct EXPRESSCPP_API FetchOptions {
  HttpMethod method = HttpMethod::Get;
  std::map<std::string, std::string> headers;
  std::string body;
};

std::string fetch(const std::string& url, const FetchOptions = {});

}  // namespace expresscpp
