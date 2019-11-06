#include "expresscpp/console.hpp"
#include "expresscpp/date.hpp"
#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "expresscpp/router.hpp"
#include "gtest/gtest.h"

using namespace expresscpp;

TEST(MiddlewareTests, LoggerLikeMiddleware) {
  ExpressCpp app;
  constexpr std::string_view message = "hello world";

  bool logger_called = false;
  bool route_called = false;
  auto LoggerMiddleware = [&](auto req, auto /*res*/, auto next) {
    const auto n = Date::getTime();
    Console::Log(
        fmt::format("time: {}, method: \"{}\", path: \"{}\"", n, getHttpMethodName(req->getMethod()), req->getPath()));
    logger_called = true;
    (*next)();
  };

  app.Use(LoggerMiddleware);

  app.Get("/a", [&](auto /*req*/, auto res, auto /*next*/) {
    route_called = true;
    res->Send(message.data());
  });

  auto stack = app.Stack();

  constexpr auto port = 8081;
  app.Listen(port, [&]() {
    EXPECT_EQ(logger_called, false);
    const auto get_response = fetch(fmt::format("http://localhost:{}/a", port), {
                                                                                    .method = HttpMethod::Get,
                                                                                });
    EXPECT_EQ(route_called, true);
    EXPECT_EQ(get_response, message);
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
    if (req->getHeaders().find("Authorization") != req->getHeaders().end()) {
      if (req->getHeaders()["Authorization"] == "secret_token") {
        authorized = true;
        // TODO(gocarlos): ugly, improveme
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

  constexpr auto port = 8081;
  app.Listen(port, [&]() {
    EXPECT_EQ(auth_called, false);
    EXPECT_EQ(authorized, false);
    {
      const auto get_response = fetch("/secret", boost::beast::http::verb::get);
      EXPECT_EQ(get_response, error_message);
      EXPECT_EQ(auth_called, true);
      EXPECT_EQ(authorized, false);
    }
    {
      std::map<std::string, std::string> headers;
      headers["Authorization"] = "secret_token";
      const auto get_response =
          fetch(fmt::format("http://localhost:{}/secret", port), {.method = HttpMethod::Get, .headers = headers});
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
      if (req->getHeaders().find("Authorization") != req->getHeaders().end()) {
        if (req->getHeaders()["Authorization"] == "secret_token") {
          authorized = true;
          // TODO(gocarlos): ugly, improveme
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

    constexpr auto port = 8081;
    app.Listen(port, [&]() {
      EXPECT_EQ(auth_called, false);
      EXPECT_EQ(authorized, false);
      {
        const auto get_response = fetch("/not_secret", boost::beast::http::verb::get);
        EXPECT_EQ(get_response, not_secret_message);
        EXPECT_EQ(auth_called, false);
        EXPECT_EQ(authorized, false);
      }
      {
        const auto get_response = fetch("/secret", boost::beast::http::verb::get);
        EXPECT_EQ(get_response, error_message);
        EXPECT_EQ(auth_called, true);
        EXPECT_EQ(authorized, false);
      }
      {
        std::map<std::string, std::string> headers;
        headers["Authorization"] = "secret_token";
        const auto get_response = fetch("/secret", boost::beast::http::verb::get, headers);
        EXPECT_EQ(get_response, secret_message);
        EXPECT_EQ(auth_called, true);
        EXPECT_EQ(authorized, true);
      }
    });
  }
}
