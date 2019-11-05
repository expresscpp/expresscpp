#include "gtest/gtest.h"

#include "expresscpp/expresscpp.hpp"
#include "test_utils.hpp"

using namespace expresscpp;

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

  app.Listen(8081, []() {
    {
      const auto get_response = getResponse("/a", boost::beast::http::verb::get);
      const auto post_response = getResponse("/a", boost::beast::http::verb::post);
      const auto put_response = getResponse("/a", boost::beast::http::verb::put);
      EXPECT_EQ(get_response, "get response");
      EXPECT_EQ(post_response, "post response");
      EXPECT_EQ(put_response, "put response");
    }
    {
      const auto get_response = getResponse("/b", boost::beast::http::verb::get);
      const auto post_response = getResponse("/b", boost::beast::http::verb::post);
      const auto put_response = getResponse("/b", boost::beast::http::verb::put);
      EXPECT_EQ(get_response, "get response b");
      EXPECT_EQ(post_response, "post response b");
      EXPECT_EQ(put_response, "put response b");
    }
  });
}
