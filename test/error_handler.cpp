#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "fmt/format.h"
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

using namespace expresscpp;

constexpr uint16_t port = 8081;

TEST(ErrorHandler, BasicErrorHandler) {
  ExpressCpp app;

  bool error_middleware_visited = false;
  std::string error_message = "Internal Error";

  auto ErrorMiddleware = [&](auto ec, auto /*req*/, auto res, auto /*next*/) {
    error_middleware_visited = true;
    std::cerr << "Internal Error: " << ec.message() << std::endl;
    res->SetStatus(500);
    res->Send(error_message);
  };

  app.Error(ErrorMiddleware);

  struct User {
    std::string name;
    uint8_t age;
  };

  std::vector<User> users{{"john", 30}, {"steve", 40}};

  app.Get("/users/:userId", [&](auto req, auto res, auto /*next*/) {
    auto params = req->GetParams();

    if ((params.find("userId") != params.end())) {
      auto id = params.at("userId");
      const int userId = std::stoi(id);

      // ops, possible the user does not exist -> exception
      auto user = users.at(userId);

      res->Json(fmt::format("{{\"name\":\"{}\", \"age\":{}}}", user.name, user.age));
    } else {
      res->SetStatus(404);
      res->Send(R"({"error":"user not found"})");
    }
  });

  app.Listen(port, [&](auto ec) {
    EXPECT_FALSE(ec);

    const auto should_work_response = fetch(fmt::format("localhost:{}/users/1", port));
    EXPECT_FALSE(error_middleware_visited);
    const auto expected = nlohmann::json::parse(should_work_response);
    EXPECT_TRUE(expected.count("name")>0);
    
    const auto should_not_work_response = fetch(fmt::format("localhost:{}/users/2", port));
    EXPECT_TRUE(error_middleware_visited);

    EXPECT_EQ(should_not_work_response, error_message);
  });
}
