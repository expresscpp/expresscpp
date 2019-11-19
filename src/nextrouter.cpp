#include "expresscpp/nextrouter.hpp"

#include "expresscpp/console.hpp"
#include "expresscpp/router.hpp"

namespace expresscpp {

void NextRouter::operator()(std::shared_ptr<std::string> error) {
  Console::Debug("next called");
  if (error != nullptr) {
    Console::Error(*error);
  }
  if (router_ == nullptr) {
    Console::Trace("no callback for next handler registered");
    return;
  }
  router_->Next(req_, res_, error);
}

NextRouter::NextRouter(Router* router, std::shared_ptr<Request> req, std::shared_ptr<Response> res)
    : req_{std::move(req)}, res_{std::move(res)}, router_{router} {
}

}  // namespace expresscpp
