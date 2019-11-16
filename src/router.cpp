#include "expresscpp/router.hpp"

#include <functional>
#include <memory>

#include "boost/algorithm/string.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"

#include "expresscpp/console.hpp"
#include "expresscpp/impl/matcher.hpp"
#include "expresscpp/impl/utils.hpp"
#include "expresscpp/layer.hpp"

using namespace std::literals;

namespace expresscpp {

Router::Router() {
  Init();
}

Router::Router(std::string_view name) : name_(name) {
  Init();
}

void Router::Init() {
  timestamp_ = std::chrono::system_clock::now();
  uuid_ = boost::uuids::random_generator()();
  Console::Debug(fmt::format("Router \"{}\" created, uuid \"{}\"", name_, boostUUIDToString(uuid_)));
}

std::vector<std::shared_ptr<Layer>> Router::stack() const {
  return stack_;
}

void Router::Use(std::string_view registered_path, handler_wn_t handler) {
  Console::Debug(fmt::format("use \"{}\" registered", registered_path));
  PathToRegExpOptions op{.sensitive = this->caseSensitive, .strict = true, .end = false};
  auto layer = std::make_shared<Layer>(registered_path, op, parent_path_, handler);
  stack_.emplace_back(layer);
}

void Router::Use(handler_wn_t handler) {
  Console::Debug(fmt::format("use middleware"));
  PathToRegExpOptions op{.sensitive = this->caseSensitive, .strict = false, .end = false};
  auto layer = std::make_shared<Layer>("/", op, parent_path_, handler);
  stack_.emplace_back(layer);
}

void Router::RegisterPath(std::string_view registered_path, const HttpMethod method, handler_wn_t handler) {
  auto route = CreateRoute(registered_path);
  Console::Debug(fmt::format("registering path \"{}\"", registered_path));
  PathToRegExpOptions op;
  auto layer = std::make_shared<Layer>("/", op, parent_path_, handler);
  layer->setMethod(method);
  route->methods_.insert(method);
  route->stack_.emplace_back(layer);
}

void Router::Get(std::string_view path, handler_wn_t handler) {
  Console::Debug(fmt::format("get \"{}\" registered", path));
  RegisterPath(path, HttpMethod::Get, handler);
}

void Router::Post(std::string_view path, handler_wn_t handler) {
  Console::Debug(fmt::format("post \"{}\" registered", path));
  RegisterPath(path, HttpMethod::Post, handler);
}

void Router::Patch(std::string_view path, handler_wn_t handler) {
  Console::Debug(fmt::format("patch \"{}\" registered", path));
  RegisterPath(path, HttpMethod::Patch, handler);
}

void Router::Delete(std::string_view path, handler_wn_t handler) {
  Console::Debug(fmt::format("delete \"{}\" registered", path));
  RegisterPath(path, HttpMethod::Delete, handler);
}

void Router::Use(std::string_view path, std::shared_ptr<Router> router) {
  Console::Debug(fmt::format("adding router to path: \"{}\"", path));
  const auto parent_path = fmt::format("{}{}", parent_path_, path);
  router->SetParentPath(parent_path);
  PathToRegExpOptions op{.sensitive = this->caseSensitive, .strict = true, .end = false};
  auto layer = std::make_shared<Layer>(path, op, parent_path_,
                                       [router](auto req, auto res, auto /*n*/) { router->HandleRequest(req, res); });
  stack_.emplace_back(layer);
}

void Router::Next(std::shared_ptr<Request> req, std::shared_ptr<Response> res, std::shared_ptr<std::string> err) {
  if (err != nullptr) {
    Console::Error(fmt::format("next error: {}", *err));
    return;
  }
  while (req->match != true && req->idx < stack_.size()) {
    req->current_layer = stack_[req->idx++];
    req->match = matchLayer(req->current_layer, req->getPath());

    req->current_route = req->current_layer->getRoute();

    // no match
    if (req->match == false) {
      Console::Debug(fmt::format("no match for path \"{}\", layer \"{}\"", req->getPath(), req->current_layer->path_));
      continue;
    }

    if (!req->current_route) {
      // process non-route handlers normally
      break;
    }

    const auto method = req->getMethod();
    const auto has_method = req->current_route->handles_method(method);

    // don't even bother matching route
    if (!has_method && method != HttpMethod::Head) {
      req->match = false;
      continue;
    }
    req->SetParams(req->current_layer->params_);
    req->SetQueryParams(req->current_layer->query_params_);
    req->SetQueryString(req->current_layer->query_string_);
  }
}

void Router::HandleRequest(std::shared_ptr<Request> req, std::shared_ptr<Response> res) {
  assert(req != nullptr);
  assert(res != nullptr);

  Console::Debug(fmt::format("dispatching request path: \"{}\", method \"{}\"", req->getPath(),
                             getHttpMethodName(req->getMethod())));

  // find next matching layer
  req->idx = 0;
  Next(req, res);
  NextRouter next_handler(this, req, res);
  while (req->match == true) {
    req->match = false;
    req->current_layer->HandleRequest(req, res, next_handler);
    // will be reset to true in the next() function if there is another matching layer
  }

  Console::Debug(fmt::format(R"(finished request: "{}", "{}")", getHttpMethodName(req->getMethod()), req->getPath()));
}

void Router::SetParentPath(const std::string& parent_path) {
  for (auto layer : stack_) {
    layer->SetParentPath(parent_path);
  }
  parent_path_ = parent_path;
}

auto Router::GetRouter() {
  Console::Debug("getting a router");
  auto r = std::make_shared<Router>();
  return r;
}

auto Router::GetRouter(std::string_view name) {
  Console::Debug(fmt::format("getting a router{}", name));
  auto r = std::make_shared<Router>(name);
  return r;
}

std::shared_ptr<Route> Router::CreateRoute(const std::string_view registered_path) {
  // create route
  auto r = std::make_shared<Route>(registered_path);

  // create layer and add it to the stack
  PathToRegExpOptions op{.sensitive = this->caseSensitive, .strict = true, .end = true};
  std::shared_ptr<Layer> l =
      std::make_shared<Layer>(registered_path, op, parent_path_, [&](auto req, auto res, auto next) {
        Console::Debug("lambda called");
        l->getRoute()->Dispatch(req, res, next);
      });

  // add route to the layer
  l->setRoute(r);

  stack_.emplace_back(l);
  return r;
}

std::string_view Router::GetName() const {
  return name_;
}

}  // namespace expresscpp
