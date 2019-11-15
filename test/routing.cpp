#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "gtest/gtest.h"

using namespace expresscpp;

constexpr uint16_t port = 8081u;

TEST(RoutingTests, DISABLED_ChainRouting) {
  ExpressCpp app;

  app.CreateRoute("/a")
      ->Get([](auto /*req*/, auto res) { res->Send("get request"); })
      .Post([](auto /*req*/, auto res) { res->Send("post request"); })
      .Put([](auto /*req*/, auto res) { res->Send("put request"); });

  app.CreateRoute("/b")
      ->Get([](auto /*req*/, auto res) { res->Send("get request b"); })
      .Post([](auto /*req*/, auto res) { res->Send("post request b"); })
      .Put([](auto /*req*/, auto res) { res->Send("put request b"); });

  app.Listen(port, [](auto ec) {
    {
      EXPECT_FALSE(ec);
      const auto get_response = fetch(fmt::format("localhost:{}/a", port), {.method = HttpMethod::Get});
      const auto post_response = fetch(fmt::format("localhost:{}/a", port), {.method = HttpMethod::Post});
      const auto put_response = fetch(fmt::format("localhost:{}/a", port), {.method = HttpMethod::Put});
      EXPECT_EQ(get_response, "get response");
      EXPECT_EQ(post_response, "post response");
      EXPECT_EQ(put_response, "put response");
    }
    {
      const auto get_response = fetch(fmt::format("localhost:{}/b", port), {.method = HttpMethod::Get});
      const auto post_response = fetch(fmt::format("localhost:{}/b", port), {.method = HttpMethod::Post});
      const auto put_response = fetch(fmt::format("localhost:{}/b", port), {.method = HttpMethod::Put});
      EXPECT_EQ(get_response, "get response b");
      EXPECT_EQ(post_response, "post response b");
      EXPECT_EQ(put_response, "put response b");
    }
  });
}
