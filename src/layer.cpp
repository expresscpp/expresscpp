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
}

Layer::Layer(const std::string_view path, PathToRegExpOptions options, express_handler_wn_t handler) {
  path_ = path;
  Init();

  handler_ = handler;
  options_ = options;
  regexp_ = pathToRegExpString(path, keys_, options);
}

void Layer::Init() {
  uuid_ = boost::uuids::random_generator()();
  Console::Debug(fmt::format(R"(Layer created for path: "{}", uuid: "{}")", path_, boostUUIDToString(uuid_)));
}

bool Layer::match(std::string_view requested_path) {
  bool match{false};
  std::smatch smatch;
  std::string result;

  if (requested_path != "") {
    // TODO(gocarlos): finish this

    // fast path non-ending match for / (any path matches)
    //    if (this.regexp.fast_slash) {
    //      this.params = {} this.path = '' return true
    //    }

    // fast path for * (everything matched in a param)
    //    if (this.regexp.fast_star) {
    //      this.params = { '0' : decode_param(path) } this.path = path return true
    //    }

    // match the path
    const std::string current_path = requested_path.data();
    if (std::regex_search(current_path, smatch, regexp_) && smatch.size() > 1) {
      match = true;
      result = smatch.str(1);
    } else {
      result = std::string("");
    }
  }

  if (match) {
    params_ = {};
    return false;
  }

  //  // store values
  //  this.params = {};
  path_ = smatch[0];

  auto keys = keys_;
  auto params = params_;

  for (std::size_t i = 1; i < smatch.length(); i++) {
    auto key = keys[i - 1];
    auto prop = key.name_;
    auto val = smatch[i];
    std::cout << "val" << val << std::endl;

    if (params.find(prop) == params.end()) {
      Console::Debug("key not found");
    } else {
      Console::Debug("key found in map ");
    }

    return true;
  }
  return false;
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
    std::cout << e.what() << std::endl;
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
