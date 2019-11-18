// https://expressjs.com/en/resources/middleware/serve-favicon.html

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"
#include "expresscpp/middleware/serve_favicon_provider.hpp"

using namespace expresscpp;

int main() {
  ExpressCpp expresscpp;

  Console::setLogLevel(LogLevel::kDebug);

  FaviconProvider favicon_provider;

  expresscpp.Use(favicon_provider);
  const uint16_t port = 8081u;

  expresscpp.Use([](auto /*req*/, auto res, auto /*next*/) {
    if (!res->ResponseSent()) {
      res->Send("resource not found");
    }
  });

  // start listening for requests and block until ctrl+C
  expresscpp
      .Listen(port,
              [=](auto ec) {
                if (ec) {
                  std::cerr << "ERROR: " << ec.message() << std::endl;
                  exit(1);
                }
                std::cout << "Example app listening on port " << port << std::endl;
                std::cout << "go to http://localhost:" << port << "/favicon.ico" << std::endl;
              })
      .Run();

  return 0;
}
