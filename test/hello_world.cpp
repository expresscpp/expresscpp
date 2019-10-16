#include "gtest/gtest.h"

#include "expresscpp/expresscpp.hpp"
#include "expresscpp/types.hpp"
#include "helper.hpp"

// Performs an HTTP GET and prints the response
TEST(HelloWorld, GetRequest) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Listen(8080, []() {
    const auto s = getResponse();
    EXPECT_GE(s.size(), 0);
  });
}

TEST(HelloWorld, UseRouter) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/", [](auto req, auto res) {
    std::cout << req->path_ << std::endl;
    res->Json(R"({"status":"ok"})");
  });

  expresscpp->Get("/api/v0/users", [](auto req, auto res) {
    std::cout << req->path_ << std::endl;
    res->Json(R"({"users":[{"id": "39811e5d-cb4b-4dcf-a857-df3813fa89f3"},{"id": "4edfc753-b817-42d5-9266-d045f8945e2e"}]})");
  });

  // TODO: add here a request and expectation

  expresscpp->Listen(8080, []() {
    const auto s = getResponse();
    EXPECT_GE(s.size(), 0);
  });
}
