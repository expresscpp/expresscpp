#include "expresscpp/nextrouter.hpp"

#include "expresscpp/console.hpp"

namespace expresscpp {

void NextRouter::operator()(std::shared_ptr<std::string> error) {
  Console::Debug("next called");
  if (error != nullptr) {
    Console::Error(*error);
  }
  if(!callback_registered_){
    Console::Trace("no callback for next handler registered");
    return;
  }
  cb_(error);
}

void NextRouter::setCallback(std::function<void(std::shared_ptr<std::string>)> cb) {
  cb_ = cb;
  callback_registered_ = true;
}

}  // namespace expresscpp
