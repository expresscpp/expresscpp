#include "expresscpp/date.hpp"

#include "gtest/gtest.h"

using namespace expresscpp;

TEST(DateTests, GetTime) {
  Date d;
  auto n = d.getTime();
  EXPECT_NE(n.size(), 0);
}
