#include "gtest/gtest.h"

#include "expresscpp/date.hpp"

using namespace expresscpp;

TEST(DateTests, GetTime) {
  Date d;
  auto n = d.getTime();
  EXPECT_NE(n.size(), 0);
}
