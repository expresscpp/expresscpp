#include "gtest/gtest.h"

#include "expresscpp/expresscpp.hpp"
#include "test_utils.hpp"

TEST(RoutingTable, TestBasicRouting) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/a", [](auto /*req*/, auto res) { res->Send("get_a"); });
  expresscpp->Get("/b", [](auto /*req*/, auto res) { res->Send("get_b"); });
  expresscpp->Get("/c", [](auto /*req*/, auto res) { res->Send("get_c"); });
  expresscpp->Post("/d", [](auto /*req*/, auto res) { res->Send("post_d"); });

  expresscpp->Listen(8080, [=]() {
    const auto a = getResponse("/a", boost::beast::http::verb::get);
    EXPECT_EQ(a, "get_a");

    const auto b = getResponse("/b", boost::beast::http::verb::get);
    EXPECT_EQ(b, "get_b");

    const auto c = getResponse("/c", boost::beast::http::verb::get);
    EXPECT_EQ(c, "get_c");

    const auto d = getResponse("/d", boost::beast::http::verb::post);
    EXPECT_EQ(d, "post_d");
  });
}

TEST(RoutingTable, TestDefaultRouting) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/a", [](auto /*req*/, auto res) { res->Send("get_a"); });

  // Return 404 on non declared routes
  const std::string error = "Error: not found";
  expresscpp->Use("*", [=](auto /*req*/, auto res) {
    res->SetStatus(404);
    res->Send(error);
  });

  expresscpp->Listen(8080, [=]() {
    const auto a = getResponse("/a", boost::beast::http::verb::get);
    EXPECT_EQ(a, "get_a");

    auto e = getResponse("/e", boost::beast::http::verb::post);
    EXPECT_EQ(e, error);
    e = getResponse("/e", boost::beast::http::verb::get);
    EXPECT_EQ(e, error);
  });
}
