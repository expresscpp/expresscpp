#pragma once

#include <functional>
#include <memory>
#include <queue>

#include "expresscpp/http_method.hpp"
#include "expresscpp/request.hpp"
#include "expresscpp/response.hpp"

typedef std::shared_ptr<Request> express_request_t;
typedef std::shared_ptr<Response> express_response_t;
typedef std::function<void(express_request_t req, express_response_t res)> express_handler_t;

class ExpressCppHandler {
 public:
  ExpressCppHandler() {}
  express_handler_t handler;
  HttpMethod method;
  std::string debug_function_name;
};

typedef std::queue<ExpressCppHandler> express_handler_queue_t;
