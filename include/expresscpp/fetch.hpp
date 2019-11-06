#pragma once

#include <map>
#include <string>

#include "boost/beast/http/verb.hpp"

namespace expresscpp {

std::string fetch(const std::string& path, boost::beast::http::verb verb = boost::beast::http::verb::get,
                  std::map<std::string, std::string> headers = {});

}
