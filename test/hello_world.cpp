#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "expresscpp/types.hpp"

using namespace std::literals;
using namespace expresscpp;

TEST(HelloWorld, UseRouter) {
  auto app = std::make_shared<ExpressCpp>();

  app->Get("/", [](auto /*req*/, auto res, auto /*next*/) {
    Console::Debug("/ called");
    res->Json(R"({"status":"ok"})");
  });

  const auto json_response = R"({"users":
                    [
                      {"id": "39811e5d-cb4b-4dcf-a857-df3813fa89f3"},
                      {"id": "4edfc753-b817-42d5-9266-d045f8945e2e"}
                    ]
                  })"sv;

  app->Get("/api/v0/users", [=](auto /*req*/, auto res, auto /*next*/) {
    Console::Debug("/api/v0/users called");

    res->Json(json_response);
  });

  app->Listen(8081, [=]() {
    const auto s = fetch("/", boost::beast::http::verb::get);
    EXPECT_EQ(s, R"({"status":"ok"})");
    const auto ss = fetch("/api/v0/users", boost::beast::http::verb::get);
    const auto expected = nlohmann::json::parse(json_response);
    const auto received = nlohmann::json::parse(ss);
    const std::string expected_string = expected.dump();
    const std::string received_string = received.dump();

    EXPECT_EQ(expected_string, received_string);
    //    std::cout << "asdfasdf" << std::endl;
  });
}
