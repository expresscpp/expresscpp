#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "expresscpp/impl/routing_stack.hpp"
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

using namespace expresscpp;
using namespace std::string_literals;

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

TEST(BasicTests, SingleRoute) {
  ExpressCpp app;
  app.Get("/", [](auto /*req*/, auto res, auto /*next*/) { res->Send("/"); });
  app.Listen(8081, []() {
    auto r = fetch("/", boost::beast::http::verb::get);
    EXPECT_EQ(r, "/");
  });
}

TEST(BasicTests, SingleRouteJson) {
  ExpressCpp app;
  app.Get("/", [](auto /*req*/, auto res, auto /*next*/) { res->Json(R"({"status": 1 })"); });
  app.Listen(8081, []() {
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

    auto path = "/"s;
    auto method = HttpMethod::Get;

    auto req = std::make_shared<Request>(path, method);
    auto res = std::make_shared<Response>(nullptr);

    std::string name = "special_header";
    std::string value = "special_header_value";
    req->setHeader(name, value);

    app.HandleRequest(req, res, nullptr);

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

    auto path = "/aaa"s;
    auto method = HttpMethod::Get;

    auto req = std::make_shared<Request>(path, method);
    auto res = std::make_shared<Response>(nullptr);

    std::string name = "special_header";
    std::string value = "special_header_value";
    req->setHeader(name, value);

    app.HandleRequest(req, res, nullptr);

    EXPECT_EQ(res->response_sent_, true);
  }
}

TEST(BasicTests, MultiRoute) {
  {
    ExpressCpp app;
    app.Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("/a"); });
    app.Get("/b", [](auto /*req*/, auto res, auto /*next*/) { res->Send("/b"); });
    app.Listen(8081, []() {
      auto ra = fetch("/a", boost::beast::http::verb::get);
      auto rb = fetch("/b", boost::beast::http::verb::get);
      EXPECT_EQ(ra, "/a");
      EXPECT_EQ(rb, "/b");
    });
  }
}
