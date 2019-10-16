#pragma once

#include <map>
#include <string>

#include "boost/beast/http/verb.hpp"

#include "expresscpp/http_method.hpp"

class Request
{
public:
 Request() {}
 Request(std::string_view path, HttpMethod method)
     : path_(path), method_(method) {}

 std::map<std::string, std::string> headers_;
 std::string_view path_;
 HttpMethod method_;
};
