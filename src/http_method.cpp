#include "expresscpp/http_method.hpp"

#include <cctype>
#include <string>
#include <string_view>

namespace expresscpp {

HttpMethod beastVerbToHttpMethod(const boost::beast::http::verb beast_verb) {
  switch (beast_verb) {
    case boost::beast::http::verb::get:
      return HttpMethod::Get;
    case boost::beast::http::verb::post:
      return HttpMethod::Post;
    case boost::beast::http::verb::delete_:
      return HttpMethod::Delete;
    case boost::beast::http::verb::patch:
      return HttpMethod::Patch;
    default:
      return HttpMethod::All;
  }
}

std::string getHttpMethodName(const HttpMethod method) {
  switch (method) {
    case HttpMethod::All:
      return "ALL";
    case HttpMethod::Get:
      return "GET";
    case HttpMethod::Post:
      return "POST";
    case HttpMethod::Put:
      return "PUT";
    case HttpMethod::Delete:
      return "DELETE";
    case HttpMethod::Patch:
      return "PATCH";
    case HttpMethod::Head:
      return "HEAD";
    default:
      throw std::runtime_error("method not found " + std::to_string(static_cast<int>(method)));
  }
}

boost::beast::http::verb getBeastVerbFromExpressVerb(const HttpMethod method) {
  switch (method) {
    case HttpMethod::All:
      return boost::beast::http::verb::unknown;
    case HttpMethod::Get:
      return boost::beast::http::verb::get;
    case HttpMethod::Post:
      return boost::beast::http::verb::post;
    case HttpMethod::Put:
      return boost::beast::http::verb::put;
    case HttpMethod::Delete:
      return boost::beast::http::verb::delete_;
    case HttpMethod::Patch:
      return boost::beast::http::verb::patch;
    case HttpMethod::Head:
      return boost::beast::http::verb::head;
    default:
      throw std::runtime_error("method not found " + std::to_string(static_cast<int>(method)));
  }
}

HttpMethod getHttpMethodFromName(const std::string_view method) {
  std::string method_s = method.data();
  std::transform(method_s.begin(), method_s.end(), method_s.begin(), ::toupper);
  if (method_s == "ALL") {
    return HttpMethod::All;
  } else if (method_s == "GET") {
    return HttpMethod::Get;
  } else if (method_s == "POST") {
    return HttpMethod::Post;
  } else if (method_s == "PATCH") {
    return HttpMethod::Patch;
  } else if (method_s == "PUT") {
    return HttpMethod::Put;
  } else if (method_s == "DELETE") {
    return HttpMethod::Delete;
  } else if (method_s == "HEAD") {
    return HttpMethod::Head;
  }

  throw std::runtime_error(std::string("method not found ") + method.data());
}

}  // namespace expresscpp
