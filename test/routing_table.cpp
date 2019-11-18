#include "expresscpp/console.hpp"
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
    EXPECT_FALSE(ec);
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
    EXPECT_FALSE(ec);
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

TEST(RoutingTests, TestNestedRouting) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_a"); });
  expresscpp->Get("/b", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_b"); });

  auto router = expresscpp->GetRouter("nested_router");
  expresscpp->Use("/t", router);
  router->Get("/a", [](auto /*req*/, auto res, auto n) { res->Send("get_ta"); });
  router->Post("/a", [](auto /*req*/, auto res, auto n) { res->Send("post_ta"); });

  expresscpp->Listen(port, [&](auto ec) {
    EXPECT_FALSE(ec);
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

TEST(RoutingTests, TestNestedRoutingWithLaterUse) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_a"); });
  expresscpp->Get("/b", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_b"); });

  auto router = expresscpp->GetRouter("nested_router");
  router->Get("/a", [](auto /*req*/, auto res, auto n) { res->Send("get_api_v0_a"); });
  router->Post("/a", [](auto /*req*/, auto res, auto n) { res->Send("post_api_v0_a"); });
  expresscpp->Use("/api/v0", router);

  expresscpp->Listen(port, [&](auto ec) {
    EXPECT_FALSE(ec);
    const auto a = fetch(fmt::format("localhost:{}/a", port), {.method = HttpMethod::Get});
    EXPECT_EQ(a, "get_a");

    auto b = fetch(fmt::format("localhost:{}/b", port), {.method = HttpMethod::Get});
    EXPECT_EQ(b, "get_b");

    auto get_api_v0_a = fetch(fmt::format("localhost:{}/api/v0/a", port), {.method = HttpMethod::Get});
    EXPECT_EQ(get_api_v0_a, "get_api_v0_a");

    auto post_api_v0_a = fetch(fmt::format("localhost:{}/api/v0/a", port), {.method = HttpMethod::Post});
    EXPECT_EQ(post_api_v0_a, "post_api_v0_a");

    // TODO(gocarlos): do some expectations here
    expresscpp->Stack();
  });
}

TEST(RoutingTests, TestMultiNestedRouting) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_a"); });
  expresscpp->Get("/b", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_b"); });

  auto router = expresscpp->GetRouter("nested_router");
  expresscpp->Use("/t", router);
  router->Get("/a", [](auto /*req*/, auto res, auto n) { res->Send("get_ta"); });
  router->Post("/a", [](auto /*req*/, auto res, auto n) { res->Send("post_ta"); });

  auto router2 = expresscpp->GetRouter("nested_router");
  expresscpp->Use("/l", router2);
  router2->Get("/a", [](auto /*req*/, auto res, auto n) { res->Send("get_la"); });
  router2->Post("/a", [](auto /*req*/, auto res, auto n) { res->Send("post_la"); });

  expresscpp->Listen(port, [&](auto ec) {
    EXPECT_FALSE(ec);
    const auto a = fetch(fmt::format("localhost:{}/a", port), {.method = HttpMethod::Get});
    EXPECT_EQ(a, "get_a");

    auto b = fetch(fmt::format("localhost:{}/b", port), {.method = HttpMethod::Get});
    EXPECT_EQ(b, "get_b");

    auto get_ta = fetch(fmt::format("localhost:{}/t/a", port), {.method = HttpMethod::Get});
    EXPECT_EQ(get_ta, "get_ta");

    auto post_ta = fetch(fmt::format("localhost:{}/t/a", port), {.method = HttpMethod::Post});
    EXPECT_EQ(post_ta, "post_ta");

    auto get_la = fetch(fmt::format("localhost:{}/l/a", port), {.method = HttpMethod::Get});
    EXPECT_EQ(get_la, "get_la");

    auto post_la = fetch(fmt::format("localhost:{}/l/a", port), {.method = HttpMethod::Post});
    EXPECT_EQ(post_la, "post_la");

    // TODO(gocarlos): do some expectations here
    expresscpp->Stack();
  });
}

TEST(RoutingTests, TestManyMultiNestedRouting) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  for (auto index = 0; index < 100; ++index) {
    expresscpp->Get(fmt::format("/{}", index),
                    [index](auto /*req*/, auto res, auto /*next*/) { res->Send(fmt::format("get{}", index)); });
    auto router = expresscpp->GetRouter("nested_router");
    expresscpp->Use(fmt::format("/{}", index), router);
    router->Get("/a", [index](auto /*req*/, auto res, auto n) { res->Send(fmt::format("get_/{}/a", index)); });
    router->Get("/b", [index](auto /*req*/, auto res, auto n) { res->Send(fmt::format("get_/{}/b", index)); });
  }

  expresscpp->Listen(port, [&](auto ec) {
    for (auto index = 0; index < 100; ++index) {
      const auto a = fetch(fmt::format("localhost:{}/{}/a", port, index), {.method = HttpMethod::Get});
      EXPECT_EQ(a, fmt::format("get_/{}/a", index));
      const auto b = fetch(fmt::format("localhost:{}/{}/b", port, index), {.method = HttpMethod::Get});
      EXPECT_EQ(b, fmt::format("get_/{}/b", index));
    }
  });
}

TEST(RoutingTests, TestDeepNestedRouting) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/a", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_a"); });
  expresscpp->Get("/b", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get_b"); });

  auto router = expresscpp->GetRouter("nested_router");
  expresscpp->Use("/t", router);
  router->Get("/a", [](auto /*req*/, auto res, auto n) { res->Send("get_ta"); });
  router->Post("/a", [](auto /*req*/, auto res, auto n) { res->Send("post_ta"); });

  auto router2 = expresscpp->GetRouter("nested_router");
  router->Use("/l", router2);
  router2->Get("/a", [](auto /*req*/, auto res, auto n) { res->Send("get_tla"); });
  router2->Post("/a", [](auto /*req*/, auto res, auto n) { res->Send("post_tla"); });

  auto router3 = expresscpp->GetRouter("nested_router");
  router2->Use("/k", router3);
  router3->Patch("/", [](auto /*req*/, auto res, auto n) { res->Send("patch_tlk"); });
  router3->Get("/a", [](auto /*req*/, auto res, auto n) { res->Send("get_tlka"); });
  router3->Post("/a", [](auto /*req*/, auto res, auto n) { res->Send("post_tlka"); });

  expresscpp->Listen(port, [&](auto ec) {
    EXPECT_FALSE(ec);
    const auto a = fetch(fmt::format("localhost:{}/a", port), {.method = HttpMethod::Get});
    EXPECT_EQ(a, "get_a");

    auto b = fetch(fmt::format("localhost:{}/b", port), {.method = HttpMethod::Get});
    EXPECT_EQ(b, "get_b");

    auto get_ta = fetch(fmt::format("localhost:{}/t/a", port), {.method = HttpMethod::Get});
    EXPECT_EQ(get_ta, "get_ta");

    auto post_ta = fetch(fmt::format("localhost:{}/t/a", port), {.method = HttpMethod::Post});
    EXPECT_EQ(post_ta, "post_ta");

    auto get_tla = fetch(fmt::format("localhost:{}/t/l/a", port), {.method = HttpMethod::Get});
    EXPECT_EQ(get_tla, "get_tla");

    auto post_tla = fetch(fmt::format("localhost:{}/t/l/a", port), {.method = HttpMethod::Post});
    EXPECT_EQ(post_tla, "post_tla");

    auto patch_tlk = fetch(fmt::format("localhost:{}/t/l/k", port), {.method = HttpMethod::Patch});
    EXPECT_EQ(patch_tlk, "patch_tlk");

    auto get_tlka = fetch(fmt::format("localhost:{}/t/l/k/a", port), {.method = HttpMethod::Get});
    EXPECT_EQ(get_tlka, "get_tlka");

    auto post_tlka = fetch(fmt::format("localhost:{}/t/l/k/a", port), {.method = HttpMethod::Post});
    EXPECT_EQ(post_tlka, "post_tlka");

    // TODO(gocarlos): do some expectations here
    expresscpp->Stack();
  });
}
