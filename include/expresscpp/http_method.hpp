#pragma once

#include "boost/beast/http/verb.hpp"

enum class HttpMethod { All, Get, Post, Put, Delete, Patch, Head, Options };

HttpMethod beastVerbToHttpMethod(const boost::beast::http::verb beast_verb);
