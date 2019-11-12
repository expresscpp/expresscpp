#pragma once

#include <chrono>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "boost/uuid/uuid.hpp"
#include "expresscpp/http_method.hpp"
#include "expresscpp/url.hpp"

namespace expresscpp {

class Route;
class Layer;

class Request {
 public:
  Request();
  Request(std::string_view path, HttpMethod method);

  int version_;

  std::string getTimeStamp() const;

  std::string getBaseUrl() const;
  void setBaseUrl(const std::string &baseUrl);

  std::string getOriginalUrl() const;
  void setOriginalUrl(const std::string &originalUrl);

  std::string getUrl() const;
  void setUrl(const std::string &url);

  std::string_view getPath() const;
  void setPath(const std::string_view &path);

  HttpMethod getMethod() const;
  void setMethod(const HttpMethod &method);

  std::shared_ptr<Route> getRoute() const;
  void setRoute(const std::shared_ptr<Route> &route);

  void setHeader(const std::string &key, const std::string &value);
  std::string getHeader(const std::string &key) const;

  std::map<std::string, std::string> getHeaders() const;

  [[nodiscard]] const std::map<std::string, std::string> &GetParams() const;

  const std::map<std::string, std::string> &GetQueryParams() const;

  const std::string &GetQueryString() const;

  void SetQueryString(const std::string &query_string);

  void SetParams(const std::map<std::string, std::string> &params);

  void SetQueryParams(const std::map<std::string, std::string> &query_params);

  bool match{false};
  std::size_t idx{0u};

  // current layer and current route which are going to be used
  std::shared_ptr<Route> current_route;
  std::shared_ptr<Layer> current_layer;

  [[nodiscard]] std::string getBody() const;
  void setBody(const std::string &body);

 private:
  void Init();
  std::chrono::system_clock::time_point timestamp_;

  boost::uuids::uuid uuid_;
  std::string_view path_;
  HttpMethod method_;
  std::string url_;
  std::string baseUrl_;
  std::string originalUrl_;

  std::map<std::string, std::string> headers_;
  std::shared_ptr<Route> route_;

  std::map<std::string, std::string> params_;
  std::map<std::string, std::string> query_params_;
  std::string query_string_;

  std::string body_;
};

typedef std::shared_ptr<Request> express_request_t;

std::string getPathname(express_request_t req);

std::optional<Url> parseUrl(express_request_t req);

Url fastparse(const std::string &str);

}  // namespace expresscpp
