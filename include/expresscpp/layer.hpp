#pragma once

#include <optional>
#include <regex>
#include <string_view>

#include "expresscpp/key.hpp"
#include "expresscpp/options.hpp"
#include "expresscpp/path_to_regexp.hpp"
#include "expresscpp/route.hpp"

namespace expresscpp {

class Layer {
 public:
  Layer(const std::string_view registered_path);
  Layer(const std::string_view registered_path, PathToRegExpOptions options, std::string_view parent_path,
        handler_wn_t handler);

  void SetParentPath(const std::string_view parent_path);

  /**
   * Check if this route matches `path`, if so
   * populate `.params`.
   * @param {String} path
   * @return {Boolean}
   */
  bool Match(std::string_view path);
  void HandleRequest(request_t req, response_t res, next_t next);

  handler_wn_t handler_;
  std::regex regexp_;
  std::string path_;
  PathToRegExpOptions options_;
  HttpMethod method_ = HttpMethod::All;
  std::vector<Key> keys_;

  // TODO(gocarlos): right now all params are std::string and have to be converted afterwards
  std::map<std::string, std::string> params_;
  std::map<std::string, std::string> query_params_;
  std::string query_string_;

  HttpMethod method() const;
  void setMethod(const HttpMethod &method);

  std::shared_ptr<Route> getRoute() const;
  void setRoute(const std::shared_ptr<Route> &new_route);

 private:
  void parseQueryString(std::string_view requested_path, size_t key_start_pos);
  void Init();
  std::shared_ptr<Route> route_;
  boost::uuids::uuid uuid_;
};

}  // namespace expresscpp
