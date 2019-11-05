#include "gtest/gtest.h"

#include "expresscpp/impl/utils.hpp"

using namespace expresscpp;

TEST(UtilsTests, BasicPathToRegex) {
  {
    std::string_view path = "/api/v0/:id";
    std::vector<Key> keys;
    Options op;
    auto r = pathToRegExpString(path, keys, op);

    EXPECT_TRUE([&]() {
      for (const auto& k : keys) {
        if (k.name_ == "id") {
          return true;
        }
      }
      return false;
    }());
  }
}

TEST(UtilsTests, DISABLED_CombinedPathToRegex) {
  {
    std::string_view path = "/api/v0/things/:id-:ip";
    std::vector<Key> keys;
    Options op;
    auto r = pathToRegExpString(path, keys, op);

    EXPECT_TRUE([&]() {
      for (const auto& k : keys) {
        if (k.name_ == "id") {
          return true;
        }
      }
      return false;
    }());
    EXPECT_TRUE([&]() {
      for (const auto& k : keys) {
        if (k.name_ == "ip") {
          return true;
        }
      }
      return false;
    }());
  }
  {
    std::string_view path = "/api/v0/things/:uuid/:id";
    std::vector<Key> keys;
    Options op;
    auto r = pathToRegExpString(path, keys, op);

    EXPECT_TRUE([&]() {
      for (const auto& k : keys) {
        if (k.name_ == "uuid") {
          return true;
        }
      }
      return false;
    }());
    EXPECT_TRUE([&]() {
      for (const auto& k : keys) {
        if (k.name_ == "id") {
          return true;
        }
      }
      return false;
    }());
  }
}
