#include "expresscpp/layer.hpp"

#include <regex>

#include "expresscpp/console.hpp"
#include "expresscpp/impl/utils.hpp"

namespace expresscpp {

Layer::Layer(const std::string_view path) { Console::Debug(fmt::format("new layer created for path {}", path)); }

Layer::Layer(const std::string_view path, Options options, express_handler_wn_t handler) {
  Console::Debug(fmt::format("new layer created for path \"{}\"", path));

  path_ = path;
  handler_ = handler;
  options_ = options;
  regexp_ = pathToRegExpString(path, keys_, options);
}

bool Layer::match(std::string_view path) {
  bool match{false};
  std::smatch smatch;
  std::string result;

  if (path != "") {
    // TODO: finish this

    // fast path non-ending match for / (any path matches)
    //    if (this.regexp.fast_slash) {
    //      this.params = {} this.path = '' return true
    //    }

    // fast path for * (everything matched in a param)
    //    if (this.regexp.fast_star) {
    //      this.params = { '0' : decode_param(path) } this.path = path return true
    //    }

    // match the path
    const std::string current_path = path.data();
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
      std::cout << "key not found" << std::endl;
    } else {
      std::cout << "key found in map " << std::endl;
    }

    return true;
  }
  return false;
}

void Layer::handle_request(express_request_t req, express_response_t res, express_next_t next) {
  Console::Debug("Layer handling request");
}

std::shared_ptr<Route> Layer::getRoute() const {
  if (route == nullptr) {
    Console::Trace("******route is nullptr*****");
  }
  return route;
}

void Layer::setRoute(const std::shared_ptr<Route> &value) { route = value; }

HttpMethod Layer::method() const { return method_; }

void Layer::setMethod(const HttpMethod &method) { method_ = method; }

}  // namespace expresscpp
