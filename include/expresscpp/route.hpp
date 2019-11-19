#pragma once

#include <set>
#include <string>
#include <string_view>

#include "boost/uuid/uuid.hpp"
#include "expresscpp/http_method.hpp"
#include "expresscpp/types.hpp"

namespace expresscpp {

class Layer;

class Route {
 public:
  //! @brief Initialize `Route` with the given `path`
  Route(std::string_view path);

  Route(std::string_view path, HttpMethod method);
  
  ~Route();

  void Dispatch(request_t req, response_t res, next_t next);

  std::string_view GetPath() const;
  void SetPath(const std::string_view& path);

  /**
   * Determine if the route handles a given method.
   */
  bool HasMethod(HttpMethod method);

  std::set<HttpMethod> methods_;

  std::vector<std::shared_ptr<Layer>> stack_;

 private:
  void Init();

  boost::uuids::uuid uuid_;
  std::string_view path_;
};

}  // namespace expresscpp
