#include "expresscpp/layer.hpp"

#include <regex>

#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "expresscpp/console.hpp"
#include "expresscpp/impl/utils.hpp"
#include "expresscpp/path_to_regexp.hpp"

namespace expresscpp {

Layer::Layer(const std::string_view registered_path) {
  path_ = registered_path;
  Init();
  PathToRegExpOptions default_options;
  regexp_ = pathToRegExpString(path_, keys_, default_options);
}

Layer::Layer(const std::string_view registered_path, PathToRegExpOptions options, std::string_view parent_path,
             handler_wn_t handler) {
  path_ = registered_path;
  Init();

  handler_ = handler;
  options_ = options;
  regexp_ = pathToRegExpString(path_, keys_, options, parent_path);
}

void Layer::SetParentPath(const std::string_view parent_path) {
  regexp_ = pathToRegExpString(path_, keys_, options_, parent_path);
}

void Layer::Init() {
  uuid_ = boost::uuids::random_generator()();
  Console::Debug(fmt::format(R"(Layer created for path: "{}", uuid: "{}")", path_, boostUUIDToString(uuid_)));
}

std::vector<Key> Layer::getKeys() const
{
    return keys_;
}

void Layer::setKeys(const std::vector<Key> &keys)
{
    keys_ = keys;
}

std::string Layer::getQuery_string() const
{
    return query_string_;
}

void Layer::setQuery_string(const std::string &query_string)
{
    query_string_ = query_string;
}

std::map<std::string, std::string> Layer::getQuery_params() const
{
    return query_params_;
}

void Layer::setQuery_params(const std::map<std::string, std::string> &query_params)
{
    query_params_ = query_params;
}

std::map<std::string, std::string> Layer::getParams() const
{
    return params_;
}

void Layer::setParams(const std::map<std::string, std::string> &params)
{
    params_ = params;
}

std::string Layer::getPath() const
{
    return path_;
}

void Layer::setPath(const std::string &path)
{
    path_ = path;
}

void Layer::parseQueryString(std::string_view requested_path, size_t key_start_pos) {
    size_t param_pair_end_pos = 0;
  size_t equal_sign_pos = 0;
  while (equal_sign_pos != std::string::npos) {
    equal_sign_pos = requested_path.find("=", equal_sign_pos + 1);
    param_pair_end_pos = requested_path.find("&", param_pair_end_pos + 1);
    if (equal_sign_pos < param_pair_end_pos) {
      auto key = requested_path.substr(key_start_pos + 1, equal_sign_pos - key_start_pos - 1);
      key_start_pos = requested_path.find("&", key_start_pos + 1);
      auto value = requested_path.substr(equal_sign_pos + 1, key_start_pos - equal_sign_pos - 1);
      query_params_[std::string(key)] = std::string(value);
    }
  }
}

bool Layer::Match(std::string_view requested_path) {
  params_.clear();
  query_params_.clear();
  query_string_.clear();
  if (requested_path.empty()) {
    return false;
  }
  const auto query_param_pos = requested_path.find("?");
  if (query_param_pos != std::string::npos) {
    parseQueryString(requested_path, query_param_pos);
    query_string_ = requested_path.substr(query_param_pos + 1);
  }
  std::string current_path;
  current_path = requested_path.substr(0, query_param_pos);
  std::smatch smatch;
  bool match = std::regex_search(current_path, smatch, regexp_);
  if (smatch.size() > 1) {
    for (const auto &key : keys_) {
      auto val = smatch[key.index_ + 1].str();
      Console::Debug(fmt::format(R"(val, "{}")", val));
      params_[key.name_] = val;
    }
  }
  return match;
}

void Layer::HandleRequest(request_t req, response_t res, next_t next) {
  Console::Debug("Layer handling request");
  if (route_ == nullptr) {
    handler_(req, res, next);
  } else {
    route_->Dispatch(req, res, next);
  }
}

std::shared_ptr<Route> Layer::getRoute() const {
  return route_;
}

void Layer::setRoute(const std::shared_ptr<Route> &new_route) {
  route_ = new_route;
}

HttpMethod Layer::method() const {
  return method_;
}

void Layer::setMethod(const HttpMethod &method) {
  method_ = method;
}

}  // namespace expresscpp
