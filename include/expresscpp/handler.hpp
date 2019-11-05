#pragma once

#include <string>

#include "expresscpp/http_method.hpp"
#include "expresscpp/types.hpp"

namespace expresscpp {

class ExpressCppHandler {
 public:
  ExpressCppHandler() = default;

  bool getIs_router() const;
  void setIs_router(bool value);

  std::string getDebug_function_name() const;
  void setDebug_function_name(const std::string &value);

  HttpMethod getMethod() const;
  void setMethod(const HttpMethod &value);

  express_handler_t handler;
  express_handler_wn_t handler_wn;

 private:
  HttpMethod method;
  std::string debug_function_name{"<anonymous>"};
  bool is_router{false};
};

typedef std::vector<ExpressCppHandler> express_handler_vector_t;
}  // namespace expresscpp
