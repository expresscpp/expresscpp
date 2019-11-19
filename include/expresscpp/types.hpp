#pragma once

#include <functional>
#include <system_error>

#include "expresscpp/nextrouter.hpp"
#include "expresscpp/request.hpp"
#include "expresscpp/response.hpp"

namespace expresscpp {

typedef std::function<void(request_t req, response_t res)> handler_t;
typedef std::function<void(request_t req, response_t res, next_t next)> handler_wn_t;
typedef std::function<void(std::error_code ec, request_t req, response_t res, next_t next)> handler_wecn_t;
typedef std::function<void(const std::error_code)> ready_fn_cb_error_code_t;

typedef std::function<void()> ready_fn_cb_void_t;

}  // namespace expresscpp
