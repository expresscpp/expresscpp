#include "expresscpp/route.hpp"

#include <iostream>
#include <string_view>

#include "boost/lexical_cast.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "expresscpp/http_method.hpp"
#include "expresscpp/impl/utils.hpp"
#include "expresscpp/layer.hpp"

namespace expresscpp {

Route::Route(std::string_view path) : path_(path) {
  Init();
}

Route::~Route() {
  Console::Debug(fmt::format(R"(Destructing the route "{}")", boostUUIDToString(uuid_)));
}

Route& Route::Get(express_handler_t h) {  // TODO(gocarlos): implement me
  Console::Error("not implemented yet");
  return *this;
}

Route& Route::Post(express_handler_t h) {  // TODO(gocarlos): implement me
  Console::Error("not implemented yet");
  return *this;
}

Route& Route::Put(express_handler_t h) {  // TODO(gocarlos): implement me
  Console::Error("not implemented yet");
  return *this;
}

void Route::Init() {
  uuid_ = boost::uuids::random_generator()();
  Console::Debug(fmt::format(R"(Route created for path: "{}", uuid:"{}")", path_, boostUUIDToString(uuid_)));
}

std::string_view Route::getPath() const {
  return path_;
}

void Route::setPath(const std::string_view& path) {
  path_ = path;
}

bool Route::handles_method(HttpMethod method) {
  if (methods_.find(HttpMethod::All) != methods_.end()) {
    return true;
  }

  if (method == HttpMethod::Head && (methods_.find(HttpMethod::Head) == methods_.end())) {
    method = HttpMethod::Get;
  }

  if (methods_.find(method) != methods_.end()) {
    return true;
  }

  return false;
}

void Route::Dispatch(express_request_t req, express_response_t res, express_next_t next) {
  Console::Debug(fmt::format("Dispatch called {}", boost::lexical_cast<std::string>(uuid_)));

  for (const auto& l : stack_) {
    if (l != nullptr) {
      l->handle_request(req, res, next);
    }
  }
}

}  // namespace expresscpp
