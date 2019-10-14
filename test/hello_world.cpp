#include "gtest/gtest.h"

#include "expresscpp/expresscpp.hpp"
#include "helper.hpp"

// Performs an HTTP GET and prints the response
TEST(HalloWorld, GetRequest) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Listen(8080, []() { getResponse(); });
}
