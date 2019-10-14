#pragma once

#include <map>
#include <string>

#include "boost/beast/http/verb.hpp"

class Request
{
public:
 Request() {}
 Request(std::string_view path, boost::beast::http::verb method)
     : path_(path), method_(method) {}

 std::map<std::string, std::string> headers_;
 std::string_view path_;
 boost::beast::http::verb method_;
};
