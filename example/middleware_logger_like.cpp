#include "expresscpp/date.hpp"
#include "expresscpp/expresscpp.hpp"

using namespace expresscpp;

int main() {
  ExpressCpp app;

  auto LogMiddleware = [&](auto req, auto /*res*/, auto next) {
    std::cerr << "[" << Date::getTime() << "]"
              << " access to path "
              << "\"" << req->getPath() << "\"" << std::endl;
    (*next)();
  };

  app.Use(LogMiddleware);

  app.Get("/users/john", [&](auto /*req*/, auto res, auto /*next*/) { res->Send("hello john!"); });
  app.Get("/users/steve", [&](auto /*req*/, auto res, auto /*next*/) { res->Send("hello steve!"); });
  constexpr uint16_t port = 8081;

  app.Listen(port,
             [&](auto ec) {
               if (ec) {
                 std::cerr << "ERROR: " << ec.message() << std::endl;
                 std::cerr << "exiting..." << std::endl;
                 exit(1);
               }
               std::cout << fmt::format(R"(you can try now: "curl http://localhost:{}/users/john")", port) << std::endl;
               std::cout << fmt::format(R"(and : "curl http://localhost:{}/users/steve")", port) << std::endl;
             })
      .Block();

  return 0;
}
