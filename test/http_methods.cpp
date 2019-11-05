#include "gtest/gtest.h"

#include "expresscpp/expresscpp.hpp"
#include "expresscpp/types.hpp"
#include "test_utils.hpp"

using namespace expresscpp;

TEST(HttpMethods, DISABLED_TestDiffentMethods) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/", [](auto /*req*/, auto res, auto next) { res->Send("get"); });
  expresscpp->Post("/", [](auto /*req*/, auto res, auto next) { res->Send("post"); });
  expresscpp->Delete("/", [](auto /*req*/, auto res, auto next) { res->Send("delete"); });
  expresscpp->Patch("/", [](auto /*req*/, auto res, auto next) { res->Send("patch"); });

  expresscpp->Listen(8081, []() {
    const auto s_get = getResponse("/", boost::beast::http::verb::get);
    const auto s_post = getResponse("/", boost::beast::http::verb::post);
    const auto s_delete = getResponse("/", boost::beast::http::verb::delete_);
    const auto s_patch = getResponse("/", boost::beast::http::verb::patch);
    EXPECT_EQ(s_get, "get");
    EXPECT_EQ(s_post, "post");
    EXPECT_EQ(s_patch, "patch");
    EXPECT_EQ(s_delete, "delete");
  });
}
