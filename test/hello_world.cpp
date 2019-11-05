#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

#include "expresscpp/expresscpp.hpp"
#include "expresscpp/types.hpp"
#include "test_utils.hpp"

using namespace std::literals;
using namespace expresscpp;

TEST(HelloWorld, DISABLED_UseRouter) {
  auto app = std::make_shared<ExpressCpp>();

  app->Get("/", [](auto req, auto res, auto next) {
    std::cout << req->getPath() << std::endl;
    res->Json(R"({"status":"ok"})");
  });

  const auto json_response = R"({"users":
                    [
                      {"id": "39811e5d-cb4b-4dcf-a857-df3813fa89f3"},
                      {"id": "4edfc753-b817-42d5-9266-d045f8945e2e"}
                    ]
                  })"sv;

  app->Get("/api/v0/users", [=](auto req, auto res, auto next) {
    std::cout << req->getPath() << std::endl;
    res->Json(json_response);
  });

  app->Listen(8081, [=]() {
    const auto s = getResponse("/", boost::beast::http::verb::get);
    EXPECT_EQ(s, R"({"status":"ok"})");
    const auto ss = getResponse("/api/v0/users", boost::beast::http::verb::get);
    const auto expected = nlohmann::json::parse(json_response);
    const auto received = nlohmann::json::parse(json_response);
    EXPECT_EQ(expected.dump(), received.dump());
  });
}
