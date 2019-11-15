#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"

using namespace expresscpp;

int main() {
  ExpressCpp app;

  auto ErrorMiddleware = [&](auto ec, auto /*req*/, auto res, auto /*next*/) {
    std::cerr << "Internal Error: " << ec.message() << std::endl;
    res->SetStatus(500);
    res->Send("Internal Error");
  };

  app.Error(ErrorMiddleware);

  struct User {
    std::string name;
    uint8_t age;
  };

  std::vector<User> users{{"john", 30}, {"steve", 40}};

  app.Get("/users/:userId", [&](auto req, auto res, auto /*next*/) {
    auto params = req->GetParams();

    if ((params.find("userId") != params.end())) {
      auto id = params.at("userId");
      const int userId = std::stoi(id);

      // ops, possible the user does not exist -> exception
      auto user = users.at(userId);

      res->Json(fmt::format("{{\"name\":\"{}\", \"age\":{}}}", user.name, user.age));
    } else {
      res->SetStatus(404);
      res->Send(R"({"error":"user not found"})");
    }
  });

  constexpr uint16_t port = 8081;

  app.Listen(port,
             [=](auto ec) {
               if (ec) {
                 std::cerr << "ERROR: " << ec.message() << std::endl;
                 std::cerr << "exiting..." << std::endl;
                 exit(1);
               }
               std::cout << fmt::format(R"(you can try now (should work): "curl http://localhost:{}/users/0")", port)
                         << std::endl;
               std::cout << fmt::format(R"(and (should fail): "curl http://localhost:{}/users/10")", port) << std::endl;
             })
      .Run();

  return 0;
}
