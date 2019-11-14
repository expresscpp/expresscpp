#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "fmt/format.h"
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

constexpr uint16_t port = 8081;

struct User {
  std::string name;
  uint8_t age;
  int id;
};

TEST(MultipleHandlers, BasicMultipleHandlers) {
  using namespace expresscpp;

  ExpressCpp app;

  bool first_middleware_visited{false};
  bool second_middleware_visited{false};
  bool third_middleware_visited{false};

  const std::string error_message = "Internal Error";
  const std::string not_allowed_msg = R"({"error":"not allowed"})";

  std::vector<User> users{{"john", 0, 30}, {"steve", 1, 40}};

  auto firstMiddleware = [&](auto req, auto res, auto next) {
    first_middleware_visited = true;
    auto headers = req->getHeaders();

    if ((headers.find("Authorization") != headers.end())) {
      (*next)();
    } else {
      res->SetStatus(401);
      res->Send(not_allowed_msg);
    }
  };

  auto secondMiddleware = [&](auto req, auto res, auto next) {
    second_middleware_visited = true;
    auto headers = req->getHeaders();

    if ((headers.find("Authorization") != headers.end()) && headers["Authorization"] == "asdf") {
      (*next)();
    } else {
      res->SetStatus(401);
      res->Send(not_allowed_msg);
    }
  };

  auto thirdMiddleware = [&](auto req, auto res, auto /*next*/) {
    third_middleware_visited = true;
    auto params = req->GetParams();

    if ((params.find("userId") != params.end())) {
      auto id = params.at("userId");
      const int userId = std::stoi(id);
      if (userId < users.size()) {
        auto user = users.at(userId);
        res->Json(fmt::format("{{\"name\":\"{}\", \"id\":\"{}\", \"age\":{}}}", user.name, user.id, user.age));
        return;
      }
    }

    res->SetStatus(404);
    res->Send(R"({"error":"user not found"})");
  };

  app.Get("/users/:userId", firstMiddleware, secondMiddleware, thirdMiddleware);

  app.Listen(port, [&](auto ec) {
    EXPECT_FALSE(ec);

    EXPECT_FALSE(first_middleware_visited);
    EXPECT_FALSE(second_middleware_visited);
    EXPECT_FALSE(third_middleware_visited);

    const auto response_1 = fetch(fmt::format("localhost:{}/users/0", port));
    EXPECT_TRUE(first_middleware_visited);
    EXPECT_FALSE(second_middleware_visited);
    EXPECT_FALSE(third_middleware_visited);
    EXPECT_EQ(response_1, not_allowed_msg);

    first_middleware_visited = false;

    const auto response_2 = fetch(fmt::format("localhost:{}/users/0", port),
                                  {.method = HttpMethod::Get, .headers = {{"Authorization", "asdf"}}});
    EXPECT_TRUE(first_middleware_visited);
    EXPECT_TRUE(second_middleware_visited);
    EXPECT_TRUE(third_middleware_visited);

    const auto expected = nlohmann::json::parse(response_2);
    EXPECT_TRUE(expected.count("name") == 1);
    EXPECT_TRUE(expected.count("id") == 1);
    EXPECT_TRUE(expected.count("age") == 1);
  });
}
