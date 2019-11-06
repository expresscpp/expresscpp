#pragma once

#include "boost/beast/http/verb.hpp"

namespace expresscpp {

enum class HttpMethod { Unknown, All, Get, Post, Put, Delete, Patch, Head, Options };

HttpMethod beastVerbToHttpMethod(const boost::beast::http::verb beast_verb);

std::string getHttpMethodName(const HttpMethod method);

HttpMethod getHttpMethodFromName(const std::string_view method);

boost::beast::http::verb getBeastVerbFromExpressVerb(const HttpMethod method);

}  // namespace expresscpp
