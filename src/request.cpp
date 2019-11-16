#include "expresscpp/request.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"

using namespace std::string_literals;

namespace expresscpp {

Request::Request() {
  Init();
}
Request::Request(std::string_view path, HttpMethod method) : path_(path), method_(method) {
  Init();
}

void Request::Init() {
  timestamp_ = std::chrono::system_clock::now();
  uuid_ = boost::uuids::random_generator()();
  originalUrl_ = path_;
}

std::string Request::getBody() const {
  return body_;
}

void Request::setBody(const std::string &body) {
  body_ = body;
}

HttpMethod Request::getMethod() const {
  return method_;
}

void Request::setMethod(const HttpMethod &method) {
  method_ = method;
}

std::shared_ptr<Route> Request::getRoute() const {
  return route_;
}

void Request::setRoute(const std::shared_ptr<Route> &route) {
  route_ = route;
}

void Request::setHeader(const std::string &key, const std::string &value) {
  headers_[key] = value;
}

std::string Request::getHeader(const std::string &key) const {
  const auto itr = headers_.find(key);
  if (itr != headers_.end()) {
    return itr->second;
  }
  return "";
}

std::map<std::string, std::string> Request::getHeaders() const {
  return headers_;
}

std::string_view Request::getPath() const {
  return path_;
}

void Request::setPath(const std::string_view &path) {
  path_ = path;
}

std::string Request::getTimeStamp() const {
  time_t now_time = std::chrono::system_clock::to_time_t(timestamp_);
  const auto gmt_time = gmtime(&now_time);
  std::stringstream ss;
  ss << std::put_time(gmt_time, "%Y-%m-%d %H:%M:%S");
  return ss.str();
}

std::string Request::getBaseUrl() const {
  return baseUrl_;
}

void Request::setBaseUrl(const std::string &baseUrl) {
  baseUrl_ = baseUrl;
}

std::string Request::getOriginalUrl() const {
  return originalUrl_;
}

void Request::setOriginalUrl(const std::string &originalUrl) {
  originalUrl_ = originalUrl;
}

std::string Request::getUrl() const {
  return url_;
}

void Request::setUrl(const std::string &url) {
  url_ = url;
}

const std::map<std::string, std::string> &Request::GetParams() const {
  return params_;
}

void Request::SetParams(const std::map<std::string, std::string> &params) {
  params_ = params;
}

const std::map<std::string, std::string> &Request::GetQueryParams() const {
  return query_params_;
}

void Request::SetQueryString(const std::string &query_string) {
  query_string_ = query_string;
}

void Request::SetQueryParams(const std::map<std::string, std::string> &query_params) {
  query_params_ = query_params;
}

const std::string &Request::GetQueryString() const {
  return query_string_;
}

}  // namespace expresscpp
