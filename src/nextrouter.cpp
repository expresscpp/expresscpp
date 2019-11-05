#include "expresscpp/nextrouter.hpp"

namespace expresscpp {

void NextRouter::operator()(std::shared_ptr<std::string> error) {
  Console::Debug("next called");
  if (error != nullptr) {
    Console::Error(*error);
    cb_(error);
  }
}

void NextRouter::setCallback(std::function<void(std::shared_ptr<std::string>)> cb) {
  cb_ = cb;
  //  cb_(std::make_shared<std::string>("asdfasdf"));
}

}  // namespace expresscpp
