#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "expresscpp/impl/routing_stack.hpp"
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

using namespace expresscpp;
using namespace std::string_literals;

std::shared_ptr<Response> RoutePath(ExpressCpp &app, std::string_view path, std::string_view test_data,
                                    HttpMethod method) {
  auto req = std::make_shared<Request>(path, method);
  auto res = std::make_shared<Response>(nullptr);
  std::string name = "special_header";
  req->setHeader(name, test_data.data());
  app.HandleRequest(req, res, nullptr);
  return res;
}

TEST(BasicTests, DumpStack) {
  {
    ExpressCpp app;
    app.Get("/", [](auto /*req*/, auto res, auto /*next*/) { res->Send("/"); });
    const auto rs = app.Stack();
    auto contains = [&]() {
      for (const auto &r : rs) {
        Console::Debug(fmt::format(R"(contains "{}")", r.path));
        if (r.path == "/") {
          return true;
        }
      }
      return false;
    }();
    EXPECT_TRUE(contains);
  }
  {
    ExpressCpp app;
    app.Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("/"); });
    app.Get("/b", [](auto /*req*/, auto res, auto /*next*/) { res->Send("/"); });
    const auto rs = app.Stack();
    auto contains_a = [&]() {
      for (const auto &r : rs) {
        if (r.path == "/a") {
          EXPECT_EQ(r.method, HttpMethod::Get);
          return true;
        }
      }
      return false;
    }();
    EXPECT_TRUE(contains_a);
    auto contains_b = [&]() {
      for (const auto &r : rs) {
        if (r.path == "/b") {
          EXPECT_EQ(r.method, HttpMethod::Get);
          return true;
        }
      }
      return false;
    }();
    EXPECT_TRUE(contains_b);
  }
}

TEST(BasicTests, DumpStackWithMiddleware) {
  ExpressCpp app;
  auto logger = [](auto /*req*/, auto /*res*/, auto next) {
    Console::Debug("function called");
    (*next)();
  };
  app.Use(logger);

  app.Get("/", [](auto /*req*/, auto res, auto /*next*/) { res->Send("/"); });
  const auto rs = app.Stack();
  std::size_t number_of_root_routes = 0;

  auto contains_routes_in_stack = [&]() {
    for (const auto &r : rs) {
      if (r.path == "/") {
        number_of_root_routes++;
        // logger has method all, route has method get
        EXPECT_TRUE(r.method == HttpMethod::Get || r.method == HttpMethod::All);
      }
    }

    if (number_of_root_routes == 2) {
      return true;
    }

    return false;
  }();

  EXPECT_TRUE(contains_routes_in_stack);
}

TEST(BasicTests, SingleRouteWithParams) {
  ExpressCpp app;
  app.Get("/:id", [](auto req, auto res, auto /*next*/) {
    EXPECT_EQ(res->GetParams().size(), 1);
    EXPECT_EQ(res->GetParams()["id"], "10");
    res->Json(R"({"status": 1 })");
  });
  app.Listen(8081, [](auto ec) {
    auto r = fetch("/10", boost::beast::http::verb::get);
    const auto expected = nlohmann::json::parse(r);
    EXPECT_EQ(expected["status"], 1);
  });
}

TEST(BasicTests, SingleRouteWithRangeParams) {
  ExpressCpp app;
  app.Get("/things/:from-:to", [](auto req, auto res, auto /*next*/) {
    EXPECT_EQ(res->GetParams().size(), 2);
    EXPECT_EQ(res->GetParams()["from"], "157");
    EXPECT_EQ(res->GetParams()["to"], "2158");
    res->Json(R"({"status": 1 })");
  });
  app.Listen(8081, [](auto ec) {
    auto r = fetch("/things/157-2158", boost::beast::http::verb::get);
    const auto expected = nlohmann::json::parse(r);
    EXPECT_EQ(expected["status"], 1);
  });
}

TEST(BasicTests, SingleRouteJson) {
  ExpressCpp app;
  app.Get("/", [](auto /*req*/, auto res, auto /*next*/) { res->Json(R"({"status": 1 })"); });
  app.Listen(8081, [](auto ec) {
    EXPECT_FALSE(ec);
    auto r = fetch("/", boost::beast::http::verb::get);
    const auto expected = nlohmann::json::parse(r);
    EXPECT_EQ(expected["status"], 1);
  });
}

TEST(BasicTests, SingleRouteWithoutBeast) {
  {
    ExpressCpp app;

    app.Get("/", [](auto req, auto res, auto /*next*/) {
      auto headers = req->getHeaders();
      EXPECT_NE(headers.find("special_header"), headers.end());

      auto value = headers.at("special_header");
      EXPECT_EQ(value, "special_header_value");

      EXPECT_ANY_THROW(res->Json(R"({"status": 1 })"););
    });

    auto res = RoutePath(app, "/", "special_header_value", HttpMethod::Get);
    EXPECT_EQ(res->response_sent_, true);
  }

  {
    ExpressCpp app;

    app.Get("/aaa", [](auto req, auto res, auto /*next*/) {
      auto headers = req->getHeaders();
      EXPECT_NE(headers.find("special_header"), headers.end());

      auto value = headers.at("special_header");
      EXPECT_EQ(value, "special_header_value");

      EXPECT_ANY_THROW(res->Json(R"({"status": 1 })"););
    });

    auto res = RoutePath(app, "/aaa", "special_header_value", HttpMethod::Get);
    EXPECT_EQ(res->response_sent_, true);
  }

  {
    ExpressCpp app;

    app.Post("/aaa", [](auto req, auto res, auto /*next*/) {
      auto headers = req->getHeaders();
      EXPECT_NE(headers.find("special_header"), headers.end());

      auto value = headers.at("special_header");
      EXPECT_EQ(value, "special_header_value");

      EXPECT_ANY_THROW(res->Json(R"({"status": 1 })"););
    });

    auto res = RoutePath(app, "/aaa", "special_header_value", HttpMethod::Post);
    EXPECT_EQ(res->response_sent_, true);
  }
}

TEST(BasicTests, PostRouteWithoutBeast) {
  {
    ExpressCpp app;

    app.Post("/aaa", [](auto req, auto res, auto /*next*/) {
      auto headers = req->getHeaders();
      EXPECT_NE(headers.find("special_header"), headers.end());

      auto value = headers.at("special_header");
      EXPECT_EQ(value, "special_header_value");

      EXPECT_ANY_THROW(res->Json(R"({"status": 1 })"););
    });

    auto res = RoutePath(app, "/aaa", "special_header_value", HttpMethod::Post);
    EXPECT_EQ(res->response_sent_, true);
  }
}

TEST(BasicTests, MultiRoute) {
  {
    ExpressCpp app;
    app.Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("/a"); });
    app.Get("/b", [](auto /*req*/, auto res, auto /*next*/) { res->Send("/b"); });
    app.Listen(8081, [](auto ec) {
      EXPECT_FALSE(ec);
      auto ra = fetch("/a", boost::beast::http::verb::get);
      auto rb = fetch("/b", boost::beast::http::verb::get);
      EXPECT_EQ(ra, "/a");
      EXPECT_EQ(rb, "/b");
    });
  }
}

TEST(BasicTests, MultipleListenCalls) {
  ExpressCpp app;
  app.Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("/a"); });
  constexpr uint16_t port = 8081u;
  app.Listen(port, [](const auto ec) {
    EXPECT_EQ(ec.value(), 0);
    EXPECT_EQ(ec.message(), "Success");
  });
  app.Listen(port, [](const auto ec) { EXPECT_EQ(ec, std::errc::already_connected); });
}

TEST(BasicTests, StartMultipleApps) {
  {
    ExpressCpp app1;
    app1.Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("/a"); });
    constexpr uint16_t port = 8081u;
    app1.Listen(port, [](std::error_code ec) {
      EXPECT_EQ(ec.value(), 0);
      EXPECT_EQ(ec.message(), "Success");
    });

    ExpressCpp app2;
    app2.Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("/a"); });
    app2.Listen(port, [](std::error_code ec) { EXPECT_NE(ec.value(), 0); });
  }
}
