#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "gtest/gtest.h"

using namespace expresscpp;

constexpr uint16_t port = 8081u;

TEST(RoutingTests, ChainRouting) {
  ExpressCpp app;

  auto a = app.Use("/a")
               ->Get([](auto /*req*/, auto res, auto) { res->Send("get request a"); })
               .Post([](auto /*req*/, auto res, auto) { res->Send("post request a"); })
               .Put([](auto /*req*/, auto res, auto) { res->Send("put request a"); });

  auto b = app.Use("/b")
               ->Get([](auto /*req*/, auto res, auto) { res->Send("get request b"); })
               .Post([](auto /*req*/, auto res, auto) { res->Send("post request b"); })
               .Put([](auto /*req*/, auto res, auto) { res->Send("put request b"); });

  app.Listen(port, [](auto ec) {
    {
      EXPECT_FALSE(ec);
      const auto get_response = fetch(fmt::format("localhost:{}/a", port), {.method = HttpMethod::Get});
      const auto post_response = fetch(fmt::format("localhost:{}/a", port), {.method = HttpMethod::Post});
      const auto put_response = fetch(fmt::format("localhost:{}/a", port), {.method = HttpMethod::Put});
      EXPECT_EQ(get_response, "get request a");
      EXPECT_EQ(post_response, "post request a");
      EXPECT_EQ(put_response, "put request a");
    }
    {
      const auto get_response = fetch(fmt::format("localhost:{}/b", port), {.method = HttpMethod::Get});
      const auto post_response = fetch(fmt::format("localhost:{}/b", port), {.method = HttpMethod::Post});
      const auto put_response = fetch(fmt::format("localhost:{}/b", port), {.method = HttpMethod::Put});
      EXPECT_EQ(get_response, "get request b");
      EXPECT_EQ(post_response, "post request b");
      EXPECT_EQ(put_response, "put request b");
    }
  });
}
