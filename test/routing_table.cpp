#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "gtest/gtest.h"

using namespace expresscpp;

constexpr uint16_t port = 8081u;

TEST(RoutingTests, TestBasicRouting) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_a"); });
  expresscpp->Get("/b", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_b"); });
  expresscpp->Get("/c", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_c"); });
  expresscpp->Post("/d", [](auto /*req*/, auto res, auto /*next*/) { res->Send("post_d"); });

  expresscpp->Listen(port, [=](auto ec) {
    const auto a = fetch(fmt::format("localhost:{}/a", port), {.method = HttpMethod::Get});
    EXPECT_EQ(a, "get_a");

    const auto b = fetch(fmt::format("localhost:{}/b", port), {.method = HttpMethod::Get});
    EXPECT_EQ(b, "get_b");

    const auto c = fetch(fmt::format("localhost:{}/c", port), {.method = HttpMethod::Get});
    EXPECT_EQ(c, "get_c");

    const auto d = fetch(fmt::format("localhost:{}/d", port), {.method = HttpMethod::Post});
    EXPECT_EQ(d, "post_d");
  });
}

TEST(RoutingTests, DISABLED_TestDefaultRouting) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_a"); });

  // Return 404 on non declared routes
  const std::string error = "Error: not found";
  expresscpp->Use("*", [=](auto /*req*/, auto res) {
    res->SetStatus(404);
    res->Send(error);
  });

  expresscpp->Listen(port, [=](auto ec) {
    const auto a = fetch(fmt::format("localhost:{}/a", port), {.method = HttpMethod::Get});
    EXPECT_EQ(a, "get_a");

    auto e = fetch(fmt::format("localhost:{}/e", port), {.method = HttpMethod::Post});
    EXPECT_EQ(e, error);
    e = fetch(fmt::format("localhost:{}/e", port), {.method = HttpMethod::Get});
    EXPECT_EQ(e, error);
    // TODO(gocarlos): do some expectations here
    expresscpp->Stack();
  });
}

TEST(RoutingTests, DISABLED_TestNestedRouting) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_a"); });
  expresscpp->Get("/b", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_b"); });

  auto router = expresscpp->GetRouter("nested_router");
  expresscpp->Use("/t", router);
  router->Get("/a", [](auto /*req*/, auto res) { res->Send("get_ta"); });
  router->Post("/a", [](auto /*req*/, auto res) { res->Send("post_ta"); });

  expresscpp->Listen(port, [=](auto ec) {
    const auto a = fetch(fmt::format("localhost:{}/a", port), {.method = HttpMethod::Get});
    EXPECT_EQ(a, "get_a");

    auto b = fetch(fmt::format("localhost:{}/b", port), {.method = HttpMethod::Get});
    EXPECT_EQ(b, "get_b");

    auto get_ta = fetch(fmt::format("localhost:{}/t/a", port), {.method = HttpMethod::Get});
    EXPECT_EQ(get_ta, "get_ta");

    auto post_ta = fetch(fmt::format("localhost:{}/t/a", port), {.method = HttpMethod::Post});
    EXPECT_EQ(post_ta, "post_ta");

    // TODO(gocarlos): do some expectations here
    expresscpp->Stack();
  });
}
