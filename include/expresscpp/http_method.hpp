#pragma once

#include "boost/beast/http/verb.hpp"
#include "expresscpp/exports.hpp"

namespace expresscpp {

enum class EXPRESSCPP_API HttpMethod { Unknown, All, Get, Post, Put, Delete, Patch, Head, Options };

HttpMethod EXPRESSCPP_API  beastVerbToHttpMethod(const boost::beast::http::verb beast_verb);

std::string EXPRESSCPP_API getHttpMethodName(const HttpMethod method);

HttpMethod EXPRESSCPP_API getHttpMethodFromName(const std::string_view method);

boost::beast::http::verb EXPRESSCPP_API getBeastVerbFromExpressVerb(const HttpMethod method);

}  // namespace expresscpp
