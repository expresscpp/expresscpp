#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"

using namespace expresscpp;

void LoggerMiddleware(request_t req, response_t /*res*/, next_t /*next*/) {
  std::cout << "LOGGER: time: " << req->getTimeStamp() << ", path: "
            << "\"" << req->getPath() << "\""
            << ", method: "
            << "\"" << getHttpMethodName(req->getMethod()) << "\"" << std::endl;
  //  next();
}

void AuthMddleware(request_t /*req*/, response_t res, NextRouter next) {
  std::cout << "exampleHandler called " << std::endl;
  //  if (req->headers.empty()) {
  next();
  //  }
  res->SetStatus(200);
}

class ThingAdmin {
 public:
  void Login(request_t /*req*/, response_t res) {
    std::cout << "thing admin called" << std::endl;
    res->Send("admining thing");
  }
  void Reboot(request_t /*req*/, response_t res) {
    std::cout << "reboot called" << std::endl;
    res->Send("rebooting");
  }
  void Update(request_t /*req*/, response_t res) {
    std::cout << "update called" << std::endl;
    res->Send("updating");
  }
};

class UserMagement {
 public:
  void Login(request_t /*req*/, response_t res) {
    std::cout << "login called" << std::endl;
    res->Send("logged in");
  }
  void Me(request_t /*req*/, response_t res) {
    std::cout << "me called" << std::endl;
    res->Send("you are great");
  }
  void Logout(request_t /*req*/, response_t res) {
    std::cout << "logout called" << std::endl;
    res->Send("logged out");
  }
};

int main() {
  std::cout << "Hello World!" << std::endl;

  Console::setLogLevel(LogLevel::kDebug);

  using namespace std::placeholders;

  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Use(LoggerMiddleware);

  expresscpp->Get("/", [](auto /*req*/, auto res, auto /*next*/) { res->Send("hello world!"); });

  // Api v0
  auto api_router = expresscpp->GetRouter("api v0 router");
  expresscpp->Use("/api/v0", api_router);

  // Api v1
  auto api_v1_router = expresscpp->GetRouter("api v1 router");
  expresscpp->Use("/api/v1", api_v1_router);

  api_v1_router->Get("/", [](auto /*req*/, auto res, auto /*next*/) { res->Send("work in progress"); });

  api_v1_router->Post("/", [](auto /*req*/, auto res, auto /*next*/) { res->Send("work in progress"); });

  auto user_v1_router = expresscpp->GetRouter("api v1 user router");
  api_v1_router->Use("/user", user_v1_router);

  user_v1_router->Get("/me", [](auto /*req*/, auto res, auto /*next*/) { res->Send("work in progress"); });

  // Things
  auto things_router = expresscpp->GetRouter("thing router");
  api_router->Use("/things", things_router);

  things_router->Get("/", [](auto /*req*/, auto res, auto /*next*/) {
    std::cout << "things called" << std::endl;
    res->Send("getting all things");
  });
  things_router->Post("/", [](auto /*req*/, auto res, auto /*next*/) {
    std::cout << "things called" << std::endl;
    res->Send("adding thing");
  });
  things_router->Get("/:thing_id/status", [](auto /*req*/, auto res, auto /*next*/) {
    std::cout << "things called" << std::endl;
    res->Send("getting status of things");
  });

  // Things admin
  auto things_admin_router = expresscpp->GetRouter("thing admin router");
  things_router->Use("/:thing_id/admin", things_admin_router);
  ThingAdmin a;
  things_admin_router->Post("/login", std::bind(&ThingAdmin::Login, &a, _1, _2));
  things_admin_router->Post("/reboot", std::bind(&ThingAdmin::Reboot, &a, _1, _2));
  things_admin_router->Post("/update", std::bind(&ThingAdmin::Update, &a, _1, _2));

  // Users
  auto user_router = expresscpp->GetRouter("user v0 router");
  api_router->Use("/user", user_router);
  UserMagement m;
  user_router->Post("/login", std::bind(&UserMagement::Login, &m, _1, _2));
  user_router->Get("/me", std::bind(&UserMagement::Me, &m, _1, _2));
  user_router->Post("/logout", std::bind(&UserMagement::Login, &m, _1, _2));

  // 404 if route not found
  expresscpp->Use("*", [](auto /*req*/, auto res) {
    res->SetStatus(404);
    res->Send("not found");
  });

  //  expresscpp->DumpOnlyRouters();

  expresscpp->Stack();
  /*
  prints:
      GET: "/debug"
      ALL: "/"
      GET: "/"
      ALL: "/api/v0"
      ALL: "*"
      ALL: "/things"
          GET: "/"
          POST: "/"
          GET: "/:thing_id/status"
          ALL: "/:thing_id/admin"
              POST: "/login"
              POST: "/reboot"
              POST: "/update"
      ALL: "/user"
          POST: "/login"
          GET: "/me"
          POST: "/logout"
  */
  // TODO(gocarlos): handle error of port already being used etc.
  constexpr uint16_t port = 8081u;
  expresscpp
      ->Listen(port,
               [](std::error_code ec) {
                 if (ec) {
                   std::cerr << "Error: " << ec.message() << std::endl;
                   exit(1);
                 }

                 std::cout << "Example app listening on port " << port << std::endl;
                 std::cout << "press CTRL+C to stop it" << std::endl;
               })
      .Run();

  return 0;
}
