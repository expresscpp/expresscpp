#include "gtest/gtest.h"

#include "expresscpp/expresscpp.hpp"

TEST(BasicTests, StartAndStop) {
  auto expresscpp = ExpressCpp::GetInstance();

  EXPECT_NE(expresscpp, nullptr);
  EXPECT_NE(expresscpp->GetRouter(), nullptr);
}
