#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "fmt/format.h"
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"
#include "test_utils.hpp"

using namespace expresscpp;

constexpr uint16_t port = 8081;

TEST(LongSession, SleepSession) {
  ExpressCpp app;

  std::string error_message = "Internal Error";

  bool visited = false;
  app.Get("/users", [&](auto /*req*/, auto res, auto /*next*/) {
    visited = true;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    res->Send("asdf");
  });

  app.Listen(port, [&](auto ec) {
    EXPECT_FALSE(ec);
    const auto should_work_response = fetch(fmt::format("localhost:{}/users", port));
    EXPECT_TRUE(visited);
  });
}

TEST(LongSession, LongResponsePayload) {
  ExpressCpp app;

  std::string error_message = "Internal Error";

  bool visited = false;
  app.Get("/users", [&](auto /*req*/, auto res, auto /*next*/) {
    visited = true;
    std::string s(500000, 'a');
    std::cout << "size of string " << s.size() << std::endl;
    res->Send(s);
  });

  app.Listen(port, [&](auto ec) {
    EXPECT_FALSE(ec);
    const auto should_work_response = fetch(fmt::format("localhost:{}/users", port));
    EXPECT_TRUE(visited);
  });
}

TEST(LongSession, LongRequestPayload) {
  TestCallSleeper sleeper(1);
  ExpressCpp app;

  std::string error_message = "Internal Error";
  constexpr uint32_t size_of_payload = 1024 * 1024;
  app.Get("/users", [&](auto req, auto res, auto /*next*/) {
    sleeper.Call();
    EXPECT_EQ(req->getBody().size(), size_of_payload);
    res->Send("ok");
  });

  app.Listen(port, [&](auto ec) {
    EXPECT_FALSE(ec);
    std::string s(size_of_payload, 'a');
    const FetchOptions op{ HttpMethod::Get,  {},  s};
    const auto should_work_response = fetch(fmt::format("localhost:{}/users", port), op);
  });
  EXPECT_TRUE(sleeper.Wait());
}
