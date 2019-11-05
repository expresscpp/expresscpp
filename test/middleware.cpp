#include "gtest/gtest.h"

#include "expresscpp/console.hpp"
#include "expresscpp/date.hpp"
#include "expresscpp/expresscpp.hpp"
#include "expresscpp/router.hpp"

#include "test_utils.hpp"

using namespace expresscpp;

TEST(MiddlewareTests, DISABLED_LoggerLikeMiddleware) {
  ExpressCpp app;
  constexpr std::string_view message = "hello world";

  bool logger_called = false;
  bool route_called = false;
  auto LoggerMiddleware = [&](auto req, auto /*res*/, auto next) {
    Date d;
    auto n = d.getTime();
    Console::Log(fmt::format("time: {}, path: ", n, req->getPath()));
    logger_called = true;
    (*next)();
  };

  app.Use(LoggerMiddleware);

  app.CreateRoute("/a")->Get([&](auto /*req*/, auto res) {
    route_called = true;
    res->Send(message.data());
  });

  app.Listen(8081, [&]() {
    EXPECT_EQ(logger_called, false);
    const auto get_response = getResponse("/a", boost::beast::http::verb::get);
    EXPECT_EQ(route_called, true);
    EXPECT_EQ(get_response, message);
    EXPECT_EQ(get_response.size(), 0);
    EXPECT_EQ(logger_called, true);
  });
}

TEST(MiddlewareTests, DISABLED_AuthLikeMiddleware) {
  ExpressCpp app;
  constexpr std::string_view error_message = "Access Denied";
  constexpr std::string_view success_message = "hello world";

  bool auth_called = false;
  bool authorized = false;
  auto AuthMiddleware = [&](auto req, auto res, auto next) {
    auth_called = true;
    if (req->headers_.find("Authorization") != req->headers_.end()) {
      if (req->headers_["Authorization"] == "secret_token") {
        authorized = true;
        // TODO: ugly, improveme
        (*next)();
      }
    } else {
      authorized = false;
      res->SetStatus(401);
      res->Send(error_message.data());
    }
  };

  app.Use(AuthMiddleware);

  app.CreateRoute("/secret")->Get([&](auto /*req*/, auto res) { res->Send(success_message.data()); });

  app.Listen(8081, [&]() {
    EXPECT_EQ(auth_called, false);
    EXPECT_EQ(authorized, false);
    {
      const auto get_response = getResponse("/secret", boost::beast::http::verb::get);
      EXPECT_EQ(get_response, error_message);
      EXPECT_EQ(auth_called, true);
      EXPECT_EQ(authorized, false);
    }
    {
      std::map<std::string, std::string> headers;
      headers["Authorization"] = "secret_token";
      const auto get_response = getResponse("/secret", boost::beast::http::verb::get, headers);
      EXPECT_EQ(get_response, success_message);
      EXPECT_EQ(auth_called, true);
      EXPECT_EQ(authorized, true);
    }
  });
}

TEST(MiddlewareTests, DISABLED_SpecialAuthLikeMiddleware) {
  {
    ExpressCpp app;
    constexpr std::string_view secret_message = "secret token";
    constexpr std::string_view not_secret_message = "hello world";
    constexpr std::string_view error_message = "Access Denied";

    bool auth_called = false;
    bool authorized = false;
    auto AuthMiddleware = [&](auto req, auto res, auto next) {
      auth_called = true;
      if (req->headers_.find("Authorization") != req->headers_.end()) {
        if (req->headers_["Authorization"] == "secret_token") {
          authorized = true;
          // TODO: ugly, improveme
          (*next)();
        }
      } else {
        authorized = false;
        res->SetStatus(401);
        res->Send(error_message.data());
      }
    };

    app.Use("/secret", AuthMiddleware);

    app.CreateRoute("/not_secret")->Get([&](auto /*req*/, auto res) { res->Send(not_secret_message.data()); });
    app.CreateRoute("/secret")->Get([&](auto /*req*/, auto res) { res->Send(secret_message.data()); });

    app.Listen(8081, [&]() {
      EXPECT_EQ(auth_called, false);
      EXPECT_EQ(authorized, false);
      {
        const auto get_response = getResponse("/not_secret", boost::beast::http::verb::get);
        EXPECT_EQ(get_response, not_secret_message);
        EXPECT_EQ(auth_called, false);
        EXPECT_EQ(authorized, false);
      }
      {
        const auto get_response = getResponse("/secret", boost::beast::http::verb::get);
        EXPECT_EQ(get_response, error_message);
        EXPECT_EQ(auth_called, true);
        EXPECT_EQ(authorized, false);
      }
      {
        std::map<std::string, std::string> headers;
        headers["Authorization"] = "secret_token";
        const auto get_response = getResponse("/secret", boost::beast::http::verb::get, headers);
        EXPECT_EQ(get_response, secret_message);
        EXPECT_EQ(auth_called, true);
        EXPECT_EQ(authorized, true);
      }
    });
  }
}
