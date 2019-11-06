#pragma once

#include <optional>
#include <regex>
#include <string_view>

#include "expresscpp/handler.hpp"
#include "expresscpp/key.hpp"
#include "expresscpp/options.hpp"
#include "expresscpp/path_to_regexp.hpp"
#include "expresscpp/route.hpp"

namespace expresscpp {

class Layer {
 public:
  Layer(const std::string_view path);
  Layer(const std::string_view path, PathToRegExpOptions options, express_handler_wn_t handler);

  /**
   * Check if this route matches `path`, if so
   * populate `.params`.
   * @param {String} path
   * @return {Boolean}
   */
  bool match(std::string_view path);
  void handle_request(express_request_t req, express_response_t res, express_next_t next);

  express_handler_wn_t handler_;
  std::regex regexp_;
  std::string path_;
  PathToRegExpOptions options_;
  HttpMethod method_ = HttpMethod::All;
  std::vector<Key> keys_;

  // TODO(gocarlos): right now all params are std::string and have to be converted afterwards
  std::map<std::string, std::string> params_;

  HttpMethod method() const;
  void setMethod(const HttpMethod &method);

  std::shared_ptr<Route> getRoute() const;
  void setRoute(const std::shared_ptr<Route> &value);

 private:
  void Init();
  std::shared_ptr<Route> route;
  boost::uuids::uuid uuid_;
};

}  // namespace expresscpp
