#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"

using namespace expresscpp;

int main() {
  ExpressCpp app;

  app.Get("/a", [](auto req, auto res) { res->Send("get_a"); });
  app.Get("/b", [](auto req, auto res, auto /*next*/) { res->Send("get_b"); });

  constexpr uint16_t port = 8081;

  app.Listen(port,
             [&](auto ec) {
               if (ec) {
                 std::cerr << "ERROR: " << ec.message() << std::endl;
                 std::cerr << "exiting..." << std::endl;
                 exit(1);
               }

               std::cout << fmt::format(R"(you can try now: "curl http://localhost:{}/a")", port) << std::endl;
             })
      .Run();

  return 0;
}
