#include "gtest/gtest.h"

#include "expresscpp/expresscpp.hpp"

TEST(BasicTests, StartAndStop) {
  auto expresscpp = ExpressCpp::GetInstance();

  EXPECT_NE(expresscpp, nullptr);
  EXPECT_NE(expresscpp->GetRouter(), nullptr);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
