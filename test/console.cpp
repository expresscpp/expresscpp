#include "gtest/gtest.h"

#include "expresscpp/console.hpp"

using namespace expresscpp;

TEST(ConsoleTests, SimpleLog) {
  Console::Log("");
  Console::Log("\n");
  Console::Log(fmt::format("{}", 42));
  Console::Log(fmt::format("{}\n", 42));
}
