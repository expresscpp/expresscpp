#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "expresscpp/middleware/serve_favicon_provider.hpp"
#include "gtest/gtest.h"

using namespace expresscpp;

constexpr uint16_t port = 8081u;

TEST(FaviconTests, ServeFavicon) {
  ExpressCpp app;

  Console::setLogLevel(LogLevel::kDebug);

  FaviconProvider favicon_provider;

  app.Use(favicon_provider);

  app.Use([](auto /*req*/, auto res, auto /*next*/) {
    if (!res->ResponseSent()) {
      res->Send("resource not found");
    }
  });

  // start listening for requests and block until ctrl+C
  app.Listen(port, [=](auto ec) {
    EXPECT_FALSE(ec);

    const auto get_response = fetch(fmt::format("localhost:{}/favicon.ico", port));
    auto expected_size = favicon_provider.GetFavicon().size();
    auto actual_size = get_response.size();
    EXPECT_EQ(expected_size, actual_size);
  });
}
