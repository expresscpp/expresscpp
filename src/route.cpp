#include "expresscpp/route.hpp"

#include <iostream>
#include <string_view>

#include "boost/lexical_cast.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "expresscpp/console.hpp"
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

void Route::Init() {
  uuid_ = boost::uuids::random_generator()();
  Console::Debug(fmt::format(R"(Route created for path: "{}", uuid:"{}")", path_, boostUUIDToString(uuid_)));
}

std::string_view Route::GetPath() const {
  return path_;
}

void Route::SetPath(const std::string_view& path) {
  path_ = path;
}

bool Route::HasMethod(HttpMethod method) {
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

void Route::Dispatch(request_t req, response_t res, next_t next) {
  Console::Debug(fmt::format("Dispatch called {}", boost::lexical_cast<std::string>(uuid_)));

  for (const auto& l : stack_) {
    if (l != nullptr) {
      l->HandleRequest(req, res, next);
    }
  }
}

}  // namespace expresscpp
