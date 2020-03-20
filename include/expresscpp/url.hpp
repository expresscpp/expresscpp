#pragma once

#include <string>

#include "expresscpp/exports.hpp"

namespace expresscpp {

struct EXPRESSCPP_API Url {
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

//! @brief used for the fetch method to convert "https://localhost:8081/asdfasdf" into this struct
struct ParsedURI {
  std::string protocol;
  std::string domain;  // only domain must be present
  std::string port;
  std::string resource;
  std::string query;  // everything after '?', possibly nothing
};

//! @brief converts "https://localhost:8081/asdfasdf" into this ParsedURI
ParsedURI parseURI(const std::string& url);

}  // namespace expresscpp
