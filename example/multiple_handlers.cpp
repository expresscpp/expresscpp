#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"

using namespace expresscpp;

int main() {
  ExpressCpp app;

  struct User {
    std::string name;
    uint8_t age;
    int id;
  };

  std::vector<User> users{{"john", 0, 30}, {"steve", 1, 40}};

  auto authenticateUser = [&](auto req, auto res, auto next) {
    auto headers = req->getHeaders();

    if ((headers.find("Authorization") != headers.end())) {
      next();
    } else {
      res->SetStatus(401);
      res->Send(R"({"error":"not allowed"})");
    }
  };

  auto authenticateMachine = [&](auto req, auto res, auto next) {
    auto headers = req->getHeaders();

    if ((headers.find("Authorization") != headers.end())) {
      next();
    } else {
      res->SetStatus(401);
      res->Send(R"({"error":"not allowed"})");
    }
  };

  auto serverUser = [&](auto req, auto res, auto /*next*/) {
    auto params = req->GetParams();

    if ((params.find("userId") != params.end())) {
      auto id = params.at("userId");
      const int userId = std::stoi(id);

      // ops, possible the user does not exist -> exception
      auto user = users.at(userId);

      res->Json(fmt::format("{{\"name\":\"{}\", \"id\":\"{}\", \"age\":{}}}", user.name, user.id, user.age));
    } else {
      res->SetStatus(404);
      res->Send(R"({"error":"user not found"})");
    }
  };

  app.Get("/users/:userId", authenticateUser, serverUser);
  app.Get("/things/:userId", authenticateUser, authenticateMachine, serverUser);

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
      .Block();

  return 0;
}
