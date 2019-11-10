#include "expresscpp/router.hpp"

#include <memory>

#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
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

void Router::Use(std::string_view path, express_handler_t handler) {
  Console::Debug(fmt::format("use \"{}\" registered", path));
  RegisterPath(path, HttpMethod::All, handler);
}

void Router::Get(std::string_view path, express_handler_t handler) {
  Console::Debug(fmt::format("get \"{}\" registered", path));
  RegisterPath(path, HttpMethod::Get, handler);
}

void Router::Post(std::string_view path, express_handler_t handler) {
  Console::Debug(fmt::format("post \"{}\" registered", path));
  RegisterPath(path, HttpMethod::Post, handler);
}

void Router::Delete(std::string_view path, express_handler_t handler) {
  Console::Debug(fmt::format("delete \"{}\" registered", path));
  RegisterPath(path, HttpMethod::Delete, handler);
}

void Router::Use(std::string_view path, std::shared_ptr<Router> router) {
  Console::Debug(fmt::format("adding router to path: \"{}\"", path));
  sub_routers.push_back({path, router});
  RegisterPath(path, HttpMethod::All, [&](auto req, auto res) { router->HandleRequest(req, res); });
}

void Router::Use(std::string_view registered_path, express_handler_wn_t handler) {
  Console::Debug("using handler for all paths");
  PathToRegExpOptions op{.sensitive = this->caseSensitive, .strict = this->strict};
  auto l = std::make_shared<Layer>(registered_path, op, handler);
  stack_.push_back(l);
}

void Router::RegisterPath(std::string_view path, HttpMethod method, express_handler_t handler) {
  Console::Debug(fmt::format("handler registered path \"{}\", method \"{}\"", path, getHttpMethodName(method)));

  //  Route r{path, method};
  //  routes_.push_back(r);

  //  ExpressCppHandler express_cpp_handler;
  //  express_cpp_handler.setMethod(method);
  //  express_cpp_handler.setDebug_function_name(typeid(handler).name());

  //  express_cpp_handler.handler = handler;
  //  handler_map_[path].push_back(express_cpp_handler);
  // TODO(gocarlos): handle path = "*" -> always call this handler e.g. logger
}

void Router::HandleRequest(std::shared_ptr<Request> req, std::shared_ptr<Response> res) {
  assert(req != nullptr);
  assert(res != nullptr);

  Console::Debug(fmt::format("dispatching request path: \"{}\", method \"{}\"", req->getPath(),
                             getHttpMethodName(req->getMethod())));

  auto protohost = ""s;
  auto removed = ""s;
  auto parentUrl = ""s;

  // manage inter-router variables
  //  auto parentParams = req.params;
  parentUrl = req->getBaseUrl().size() != 0 ? req->getBaseUrl() : "";
  if (req->getOriginalUrl().length() == 0) {
    req->setOriginalUrl(req->getUrl());
  }

  // // layer and route which are going to be used
  // std::shared_ptr<Route> route;
  // std::shared_ptr<Layer> layer;

  // find next matching layer
  auto layerError = ""s;

  auto next = [=](std::shared_ptr<std::string> err = nullptr) {
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
        Console::Debug(
            fmt::format("no match for path \"{}\", layer \"{}\"", req->getPath(), req->current_layer->path_));
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
      res->SetParams(req->current_layer->params_);
      res->SetQueryParams(req->current_layer->query_params_);
      res->SetQueryString(req->current_layer->query_string_);

      // store route for dispatch on change
      //      if (route) {
      //        req->setRoute(route);
      //      }
    }
  };

  next();
  auto next_handler = std::make_shared<NextRouter>();

  while (req->match == true) {
    req->match = false;

    next_handler->setCallback(next);
    req->current_layer->handle_request(req, res, next_handler);
    // will be reset to true in the next() function if there is another matching layer
  }

  Console::Debug(fmt::format(R"(finished request: "{}", "{}")", getHttpMethodName(req->getMethod()), req->getPath()));
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
  PathToRegExpOptions op;
  std::shared_ptr<Layer> l = std::make_shared<Layer>(registered_path, op, [&](auto req, auto res, auto next) {
    Console::Debug("lambda called");
    l->getRoute()->Dispatch(req, res, next);
    //    assert(r != nullptr);
    //    r->Dispatch(req, res, next);
  });

  // add route to the layer
  l->setRoute(r);

  stack_.push_back(l);
  return r;
}

void Router::printRoutes() const {
  //  static int intendation = -1;
  //  intendation++;

  //  std::cout << "Router: " << name_ << std::endl;

  //  // loop through all routes for this router
  //  for (const auto& v : routes_) {
  //    std::string a = "";
  //    for (int i = 0; i < intendation * 4; i++) {
  //      a += " ";
  //    }

  //    // print this route for this router
  //    std::cout << a << v.getMethodName() << ": \"" << v.getPath() << "\"" << std::endl;

  //    for (const auto& rs : sub_routers) {
  //      if (rs.first != "" && rs.first == v.getPath()) {
  //        //          std::cout << "subpath is " << rs.first << std::endl;
  //        rs.second->printRoutes();
  //      }
  //    }
  //  }
  //  //    if (r.second->routers.size() > 0) {
  //  //      //    std::cout << "router has subrouters" << std::endl;
  //  //      for (const auto& subrouter : r.second->routers) {
  //  //        printRouters(subrouter, r.second->routers);
  //  //      }
  //  //    }

  //  intendation--;
}

}  // namespace expresscpp
