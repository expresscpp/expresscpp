#pragma once

#include <string>

#include "expresscpp/http_method.hpp"

namespace expresscpp {

struct RoutingStack {
  std::string path;
  HttpMethod method;
};

}  // namespace expresscpp
