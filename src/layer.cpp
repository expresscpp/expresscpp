#include "expresscpp/layer.hpp"

#include <regex>

#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "expresscpp/console.hpp"
#include "expresscpp/impl/utils.hpp"
#include "expresscpp/path_to_regexp.hpp"

namespace expresscpp {

Layer::Layer(const std::string_view path) {
  path_ = path;
  Init();
  PathToRegExpOptions default_options;
  regexp_ = pathToRegExpString(path, keys_, default_options);
}

Layer::Layer(const std::string_view registered_path, PathToRegExpOptions options, express_handler_wn_t handler) {
  path_ = registered_path;
  Init();

  handler_ = handler;
  options_ = options;
  regexp_ = pathToRegExpString(registered_path, keys_, options);
}

void Layer::Init() {
  uuid_ = boost::uuids::random_generator()();
  Console::Debug(fmt::format(R"(Layer created for path: "{}", uuid: "{}")", path_, boostUUIDToString(uuid_)));
}

bool Layer::match(std::string_view requested_path) {
  bool match{false};
  std::smatch smatch;
  const std::string current_path = requested_path.data();
  params_.clear();

  if (requested_path != "") {
    match = std::regex_search(current_path, smatch, regexp_);
    if (smatch.size() > 1) {
      for (const auto key : keys_) {
        auto val = smatch[key.index_ + 1].str();
        Console::Debug(fmt::format(R"(val, "{}")", val));
        params_[key.name_] = val;
      }
    }
  }
  return match;
}

void Layer::handle_request(express_request_t req, express_response_t res, express_next_t next) {
  Console::Debug("Layer handling request");
  try {
    if (route == nullptr) {
      handler_(req, res, next);
    } else {
      route->Dispatch(req, res, next);
    }
  } catch (std::exception &e) {
    Console::Error(e.what());
  }
}

std::shared_ptr<Route> Layer::getRoute() const {
  // if (route == nullptr) {
  //   Console::Trace("******route is nullptr*****");
  // }
  return route;
}

void Layer::setRoute(const std::shared_ptr<Route> &value) {
  route = value;
}

HttpMethod Layer::method() const {
  return method_;
}

void Layer::setMethod(const HttpMethod &method) {
  method_ = method;
}

}  // namespace expresscpp
