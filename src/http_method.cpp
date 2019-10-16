#include "expresscpp/http_method.hpp"

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
