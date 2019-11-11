#include "expresscpp/expresscpp.hpp"

using namespace expresscpp;

int main() {
  ExpressCpp app;

  app.Get("/things/:id", [](auto req, auto res, auto /*next*/) {
    std::cout << "params: " << std::endl;
    for (const auto& [name, value] : req->GetParams()) {
      std::cout << "    name: " << name << ", value: " << value << std::endl;
    }

    std::cout << "query: " << std::endl;
    for (const auto& [name, value] : req->GetQueryParams()) {
      std::cout << "    name: " << name << ", value: " << value << std::endl;
    }

    res->Json(R"({ "status": "ok" })");
  });

  constexpr uint16_t port = 8081;

  app.Listen(port,
             [=](auto ec) {
               if (ec) {
                 std::cerr << "ERROR: " << ec.message() << std::endl;
                 std::cerr << "exiting..." << std::endl;
                 exit(1);
               }
               std::cout << fmt::format(R"(you can try now: "curl http://localhost:{}/things/1234?key1=value1")", port)
                         << std::endl;
             })
      .Block();

  return 0;
}
