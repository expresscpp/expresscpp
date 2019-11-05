#include "expresscpp/router.hpp"

#include <memory>

#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"

#include "expresscpp/impl/matcher.hpp"
#include "expresscpp/layer.hpp"

using namespace std::literals;

namespace expresscpp {

Router::Router() { Init(); }

Router::Router(std::string_view name) : name_(name) { Init(); }

void Router::Init() {
  timestamp_ = std::chrono::system_clock::now();
  uuid_ = boost::uuids::random_generator()();
}

std::vector<std::shared_ptr<Layer>> Router::stack() const { return stack_; }

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

void Router::Use(std::string_view path, express_handler_wn_t handler) {
  Console::Debug("using handler for all paths");
  Options op{.caseSensitive = this->caseSensitive, .strict = this->strict};
  auto l = std::make_shared<Layer>(path, op, handler);
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
  // TODO: handle path = "*" -> always call this handler e.g. logger
}

void Router::HandleRequest(std::shared_ptr<Request> req, std::shared_ptr<Response> res) {
  Console::Debug(fmt::format("ROUTER: router HandleRequest {}:{}", __FILE__, __LINE__));

  assert(req != nullptr);
  assert(res != nullptr);

  Console::Debug(fmt::format("dispatching request for path: \"{}\" and method \"{}\"", req->getPath(),
                             getHttpMethodName(req->getMethod())));

  std::size_t idx = 0;
  auto slashAdded = false;
  auto protohost = ""s;
  auto removed = ""s;

  // manage inter-router variables
  //  auto parentParams = req.params;
  std::string parentUrl = req->getBaseUrl().size() != 0 ? req->getBaseUrl() : "";

  std::shared_ptr<Route> route;
  std::shared_ptr<Layer> layer;

  auto next = [&](std::shared_ptr<std::string> err) {
    if (err != nullptr) {
      Console::Error(*err);
      return;
    }

    auto layerError = *err == "route" ? "" : *err;

    // remove added slash
    if (slashAdded) {
      auto url = req->getUrl();
      url.pop_back();
      req->setUrl(url);
      slashAdded = false;
    }

    // restore altered req.url
    if (removed.length() != 0) {
      req->setBaseUrl(parentUrl);
      auto url = req->getUrl();
      auto new_url = url.substr(protohost.length());
      req->setUrl(protohost + removed + new_url);
      removed = "";
    }

    // no more matching layers
    if (idx >= stack_.size()) {
      Console::Debug("no more matching layers");
      //      setImmediate(done, layerError);
      return;
    }

    // get pathname of request
    auto path = getPathname(req);

    if (path == "") {
      Console::Error(layerError);
      return;
    }

    // find next matching layer
    auto match{false};

    while (match != true && idx < stack_.size()) {
      layer = stack_[idx++];
      match = matchLayer(layer, path);
      route = layer->getRoute();

      //      if (typeof match != = 'boolean') {
      //        // hold on to layerError
      //        layerError = layerError || match;
      //      }

      if (match != true) {
        continue;
      }

      if (!route) {
        // process non-route handlers normally
        continue;
      }

      if (layerError.length()) {
        // routes do not match with a pending error
        match = false;
        continue;
      }

      auto method = req->getMethod();
      auto has_method = route->handles_method(method);

      // build up automatic options response
      //      if (!has_method && method == HttpMethod::Options) {
      //        appendMethods(options, route._options());
      //      }

      // don't even bother matching route
      if (!has_method && method != HttpMethod::Head) {
        match = false;
        continue;
      }
    }

    // no match
    if (match != true) {
      Console::Error(layerError);
      return;
    }

    // store route for dispatch on change
    if (route) {
      req->setRoute(route);
    }

    //    // this should be done for the layer
    //    process_params(layer, paramcalled, req, res, [&](std::shared_ptr<std::string> err) {
    //      if (err == nullptr) {
    //        next(layerError || *err);
    //        return;
    //      }

    //      if (route) {
    //        return layer->handle_request(req, res, next);
    //      }

    //      trim_prefix(layer, layerError, layerPath, path);
    //    });
  };
  if (route) {
    express_next_t next_handler;
    next_handler->setCallback(next);
    return layer->handle_request(req, res, next_handler);
  }

  //  trim_prefix(layer, layerError, layerPath, path);
}

// void Router::process_params(std::shared_ptr<Layer> layer, ) {}

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

std::shared_ptr<Route> Router::CreateRoute(const std::string_view path) {
  // create route
  auto r = std::make_shared<Route>(path);

  // HACK
  auto lambda = [&](auto req, auto res, auto next) {
    Console::Debug("lambda called");
    assert(r != nullptr);
    r->Dispatch(req, res, next);
  };

  Options op;
  auto l = std::make_shared<Layer>(path, op, lambda);

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
