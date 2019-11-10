#include "expresscpp/handler.hpp"

namespace expresscpp {

std::string ExpressCppHandler::getDebug_function_name() const {
  return debug_function_name;
}

void ExpressCppHandler::setDebug_function_name(const std::string &value) {
  debug_function_name = value;
}

HttpMethod ExpressCppHandler::getMethod() const {
  return method;
}

void ExpressCppHandler::setMethod(const HttpMethod &value) {
  method = value;
}

bool ExpressCppHandler::getIs_router() const {
  return is_router;
}

void ExpressCppHandler::setIs_router(bool value) {
  is_router = value;
}

}  // namespace expresscpp
