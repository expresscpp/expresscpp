#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "expresscpp/expresscpp.hpp"

// void exampleHandler(std::shared_ptr<Request> req, std::shared_ptr<Response> res, NextRouter next)
//{
//    std::cout << "exampleHandler called " << std::endl;
//    if (req->headers.empty()) {
//        next();
//    }
//    res->SetStatus(200);
//}

int main() {
  std::cout << "Hello World!" << std::endl;

  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/", [](auto /*req*/, auto res) { res->Send("hello world!"); });

  expresscpp->Post("/", [](auto /*req*/, auto res) { res->Json("hello world!"); });
  expresscpp->Get("/adsf", [&](auto /*req*/, auto res) { res->Send("hello world!"); });

  expresscpp->Get("*", [](auto /*req*/, auto res) { res->Json(R"({"status":"not found"})"); });
  //    auto router = expresscpp->GetRouter();
  //    router->Use("/api/v0/healthstatus", exampleHandler);
  //    router->Post("/api/v0/login", []() { std::cout << "lambda handler called " << std::endl; });

  const uint16_t port = 8080u;
  expresscpp->Listen(port, []() { std::cout << "Example app listening on port " << port << std::endl; }).Block();

  return 0;
}
