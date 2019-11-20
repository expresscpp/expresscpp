#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"

using namespace expresscpp;

int main() {
  ExpressCpp app;
  constexpr std::string_view error_message = "Access Denied";
  constexpr std::string_view success_message = "hello you!";

  auto AuthMiddleware = [&](auto req, auto res, auto next) {
    auto headers = req->getHeaders();
    if ((headers.find("Authorization") != headers.end()) && (req->getHeaders()["Authorization"] == "secret_token")) {
      next();
    } else {
      std::cerr << "Access denied" << std::endl;
      res->SetStatus(401);
      res->Send(error_message.data());
    }
  };

  app.Use(AuthMiddleware);

  app.Get("/users/me", [&](auto /*req*/, auto res, auto /*next*/) { res->Send(success_message.data()); });
  constexpr uint16_t port = 8081;

  app.Listen(
         port,
         [&](auto ec) {
           if (ec) {
             std::cerr << "ERROR: " << ec.message() << std::endl;
             std::cerr << "exiting..." << std::endl;
             exit(1);
           }
           std::cout
               << fmt::format(
                      R"(you can try now: "curl --header "Authorization: wrong_secret_token"  http://localhost:{}/users/me")",
                      port)
               << std::endl;

           std::cout << fmt::format(
                            R"(and : "curl --header "Authorization: secret_token"  http://localhost:{}/users/me")",
                            port)
                     << std::endl;
         })
      .Run();

  return 0;
}
