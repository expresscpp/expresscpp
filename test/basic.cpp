#include "gtest/gtest.h"

#include "expresscpp/expresscpp.hpp"

TEST(BasicTests, StartAndStop) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  EXPECT_NE(expresscpp, nullptr);
  EXPECT_NE(expresscpp->GetRouter(), nullptr);
  expresscpp.reset();
}
