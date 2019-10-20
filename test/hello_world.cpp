#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

#include "expresscpp/expresscpp.hpp"
#include "expresscpp/types.hpp"
#include "test_utils.hpp"

using namespace std::literals;

// Performs an HTTP GET and prints the response
TEST(HelloWorld, GetRequest) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Listen(8081, []() {
    const auto s = getResponse("/", boost::beast::http::verb::get);
    EXPECT_GE(s.size(), 0);
  });
}

TEST(HelloWorld, UseRouter) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/", [](auto req, auto res) {
    std::cout << req->path_ << std::endl;
    res->Json(R"({"status":"ok"})");
  });

  const auto json_response = R"({"users":
                    [
                      {"id": "39811e5d-cb4b-4dcf-a857-df3813fa89f3"},
                      {"id": "4edfc753-b817-42d5-9266-d045f8945e2e"}
                    ]
                  })"sv;

  expresscpp->Get("/api/v0/users", [=](auto req, auto res) {
    std::cout << req->path_ << std::endl;
    res->Json(json_response);
  });

  expresscpp->Listen(8081, [=]() {
    const auto s = getResponse("/", boost::beast::http::verb::get);
    EXPECT_EQ(s, R"({"status":"ok"})");
    const auto ss = getResponse("/api/v0/users", boost::beast::http::verb::get);
    const auto expected = nlohmann::json::parse(json_response);
    const auto received = nlohmann::json::parse(json_response);
    EXPECT_EQ(expected.dump(), received.dump());
  });
}
