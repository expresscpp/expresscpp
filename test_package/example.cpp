#include <cstdlib>
#include <iostream>

#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"

using namespace expresscpp;

int main() {
  ExpressCpp app;

  app.Get("/", [](auto req, auto res, auto next) { res->Send("hello world!"); });

  constexpr uint16_t port = 8081;
  app.Listen(port, [=](auto ec) {
    std::cout << "listening on port " << port << std::endl;
    auto response = fetch("/");
    std::cout << "response from \"/\" => \"" << response << "\"" << std::endl;
  });
}