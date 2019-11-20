#include "expresscpp/url.hpp"

#include "gtest/gtest.h"

using namespace expresscpp;

TEST(UrlParser, ParseURL) {
  const auto url_struct = parseURI("https://localhost:8080/asdf");
  EXPECT_TRUE(url_struct.domain == "localhost");

  auto url_struct_broken = parseURI("https://:/asdf");
  EXPECT_TRUE(url_struct_broken.domain == "");
}