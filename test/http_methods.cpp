#include "gtest/gtest.h"

#include "expresscpp/expresscpp.hpp"
#include "expresscpp/types.hpp"
#include "helper.hpp"

TEST(HttpMethods, GetRequest) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/", [](auto /*req*/, auto res) { res->Send("get"); });
  expresscpp->Post("/", [](auto /*req*/, auto res) { res->Send("post"); });
  expresscpp->Delete("/", [](auto /*req*/, auto res) { res->Send("delete"); });
  expresscpp->Patch("/", [](auto /*req*/, auto res) { res->Send("patch"); });

  expresscpp->Listen(8080, []() {
    const auto s_get = getResponse("/", boost::beast::http::verb::get);
    const auto s_post = getResponse("/", boost::beast::http::verb::post);
    const auto s_delete = getResponse("/", boost::beast::http::verb::delete_);
    const auto s_patch = getResponse("/", boost::beast::http::verb::patch);
    EXPECT_GE(s_get, "get");
    EXPECT_GE(s_post, "post");
    EXPECT_GE(s_patch, "patch");
    EXPECT_GE(s_delete, "delete");
  });
}
