#pragma once

#include <map>
#include <string>

#include "boost/beast/http/verb.hpp"

// TODO(gocarlos): rename to fetch
std::string getResponse(const std::string& path, boost::beast::http::verb verb,
                        std::map<std::string, std::string> headers = {});
