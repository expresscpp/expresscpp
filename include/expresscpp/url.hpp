#pragma once

#include <string>

namespace expresscpp {

struct Url {
  std::string host;
  std::string hostname;
  std::string pathname;
  std::string href;
  std::string path;
  std::uint16_t port;
  std::string protocol;
  std::string query;
  std::string search;
  std::string slashes;
};

}  // namespace expresscpp
