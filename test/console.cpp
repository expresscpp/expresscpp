#include "expresscpp/console.hpp"

#include "gtest/gtest.h"

using namespace expresscpp;

TEST(ConsoleTests, SimpleLog) {
  std::cout << "" << std::endl;
  Console::Log("");
  Console::Log("\n");
  Console::Log(fmt::format("{}", 42));
  Console::Log(fmt::format("{}\n", 42));
  EXPECT_TRUE(true);
}
