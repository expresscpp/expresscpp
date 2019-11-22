#pragma once

#include <optional>
#include <regex>
#include <string_view>

#include "expresscpp/handlerfunctor.hpp"
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

  Layer(const std::string_view registered_path, PathToRegExpOptions options, std::string_view parent_path,
        handler_t handler);

  void SetParentPath(const std::string_view parent_path);

  /**
   * Check if this route matches `path`, if so
   * populate `.params`.
   * @param {String} path
   * @return {Boolean}
   */
  bool Match(std::shared_ptr<Request> request);
  void HandleRequest(request_t req, response_t res, next_t next);

  HttpMethod GetMethod() const;
  void SetMethod(const HttpMethod &GetMethod);

  std::shared_ptr<Route> GetRoute() const;
  void SetRoute(const std::shared_ptr<Route> &new_route);

  std::vector<Key> GetKeys() const;
  void SetKeys(const std::vector<Key> &keys);

  const std::string_view GetPath() const;

 private:
  std::map<std::string, std::string> ParseQueryString(std::string_view requested_path, size_t key_start_pos);
  void Init();
  HandlerFunctor handler_;
  std::regex regexp_;
  std::string path_;
  PathToRegExpOptions options_;
  HttpMethod method_ = HttpMethod::All;
  std::vector<Key> keys_;
  std::shared_ptr<Route> route_;
  boost::uuids::uuid uuid_;
};

}  // namespace expresscpp
