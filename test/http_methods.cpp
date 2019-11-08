#include "gtest/gtest.h"

#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "expresscpp/types.hpp"

using namespace expresscpp;

TEST(HttpMethods, TestDiffentMethods) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Get("/", [](auto /*req*/, auto res, auto /*next*/) { res->Send("get"); });
  expresscpp->Post("/", [](auto /*req*/, auto res, auto /*next*/) { res->Send("post"); });
  expresscpp->Delete("/", [](auto /*req*/, auto res, auto /*next*/) { res->Send("delete"); });
  expresscpp->Patch("/", [](auto /*req*/, auto res, auto /*next*/) { res->Send("patch"); });

  constexpr uint16_t port = 8081u;
  const std::string target = fmt::format("http://localhost:{}/", port);

  expresscpp->Listen(port, [&](auto ec) {
    const auto s_get = fetch(target, {.method = HttpMethod::Get});
    const auto s_post = fetch(target, {.method = HttpMethod::Post});
    const auto s_delete = fetch(target, {.method = HttpMethod::Delete});
    const auto s_patch = fetch(target, {.method = HttpMethod::Patch});
    EXPECT_EQ(s_get, "get");
    EXPECT_EQ(s_post, "post");
    EXPECT_EQ(s_patch, "patch");
    EXPECT_EQ(s_delete, "delete");
  });
}
