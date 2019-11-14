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

  std::string getMethodName() const;

  Route& Get(express_handler_t h);

  Route& Post(express_handler_t h);

  Route& Put(express_handler_t h);

  void Dispatch(express_request_t req, express_response_t res, express_next_t next);

  HttpMethod getMethod() const;
  void setMethod(const HttpMethod& method);

  std::string_view getPath() const;
  void setPath(const std::string_view& path);

  /**
   * Determine if the route handles a given method.
   */
  bool handles_method(HttpMethod method);

  std::set<HttpMethod> methods_;

  std::vector<std::shared_ptr<Layer>> stack_;

 private:
  void Init();

  boost::uuids::uuid uuid_;
  std::string_view path_;
};
}  // namespace expresscpp
