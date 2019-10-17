#pragma once

#include <string>

#include "boost/beast/http/verb.hpp"

std::string getResponse(const std::string& path, boost::beast::http::verb verb);
