#pragma once

#include "expresscpp/types.hpp"

namespace expresscpp {

class HandlerFunctor {
 public:
  HandlerFunctor(){};

  HandlerFunctor(handler_wn_t handler) {
    handler_with_next_ = handler;
    with_next = true;
  };

  HandlerFunctor(handler_t handler) {
    handler_ = handler;
    with_next = false;
  };

  void operator()(request_t req, response_t res) {
    handler_(req, res);
  }

  void operator()(request_t req, response_t res, next_t next) {
    handler_with_next_(req, res, next);
  }

  bool getWith_next() const;

 private:
  bool with_next{false};

  handler_t handler_;
  handler_wn_t handler_with_next_;
};

}  // namespace expresscpp
