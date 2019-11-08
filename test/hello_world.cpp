#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "expresscpp/types.hpp"
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"
#include "test_utils.hpp"

using namespace std::literals;
using namespace expresscpp;

TEST(HelloWorld, UseRouter) {
  TestCallSleeper sleeper(2);
  ExpressCpp app;

  app.Get("/", [&](auto /*req*/, auto res, auto /*next*/) {
    Console::Debug("/ called");
    res->Json(R"({"status":"ok"})");
    sleeper.Call();
  });

  const auto json_response = R"({"users":
                    [
                      {"id": "39811e5d-cb4b-4dcf-a857-df3813fa89f3"},
                      {"id": "4edfc753-b817-42d5-9266-d045f8945e2e"}
                    ]
                  })"sv;

  app.Get("/api/v0/users", [&](auto /*req*/, auto res, auto /*next*/) {
    Console::Debug("/api/v0/users called");
    res->Json(json_response);
    sleeper.Call();
  });

  constexpr uint16_t port = 8081u;
  app.Listen(port, [&](auto ec) {
    const auto s = fetch("/", boost::beast::http::verb::get);
    EXPECT_EQ(s, R"({"status":"ok"})");
    const auto ss = fetch("/api/v0/users", boost::beast::http::verb::get);
    const auto expected = nlohmann::json::parse(json_response);
    const auto received = nlohmann::json::parse(ss);
    const std::string expected_string = expected.dump();
    const std::string received_string = received.dump();
    EXPECT_EQ(expected_string, received_string);
  });
  EXPECT_TRUE(sleeper.Wait());
}

TEST(HelloWorld, UseRouterWithParams) {
  TestCallSleeper sleeper(1);
  ExpressCpp app;
  app.Get("/things/:id", [&](auto /*req*/, auto res, auto /*next*/) {
    Console::Debug("/ called");
    EXPECT_EQ(res->GetParams().size(), 1);
    EXPECT_EQ(res->GetParams()["id"], "198");
    res->Json(R"({"status":"ok"})");
    sleeper.Call();
  });

  constexpr uint16_t port = 8081u;
  app.Listen(port, [&](auto ec) {
    const auto ss = fetch("/things/198", boost::beast::http::verb::get);
    EXPECT_EQ(ss, R"({"status":"ok"})");
  });

  EXPECT_TRUE(sleeper.Wait());
}
