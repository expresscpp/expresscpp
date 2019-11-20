#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"

using namespace expresscpp;

int main() {
  ExpressCpp app;

  app.Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_a"); });
  app.Get("/b", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_b"); });

  auto router = app.GetRouter();
  router->Use([](auto req, auto res, auto next) {
    if (req->getHeader("Authorization") == "secret_token") {
      return next();
    }
    res->SetStatus(401);
    res->Send("Access denied");
  });
  router->Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_api_v0_users_a"); });
  router->Get("/b", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_api_v0_users_b"); });

  app.Use("/api/v0/users", router);

  constexpr uint16_t port = 8081;

  app.Listen(port,
             [&](auto ec) {
               if (ec) {
                 std::cerr << "ERROR: " << ec.message() << std::endl;
                 std::cerr << "exiting..." << std::endl;
                 exit(1);
               }

               std::cout << fmt::format(R"(you can try now: "curl http://localhost:{}/api/v0/users")", port)
                         << std::endl;
             })
      .Run();

  return 0;
}