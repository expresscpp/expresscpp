#include "expresscpp/path_to_regexp.hpp"

#include "gtest/gtest.h"

using namespace expresscpp;

TEST(PathToRegExpTests, BasicPathToRegex) {
  {
    std::string_view path = "/api/v0/:id";
    std::vector<Key> keys;
    PathToRegExpOptions op;
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

TEST(PathToRegExpTests, DISABLED_CombinedPathToRegex) {
  {
    std::string_view path = "/api/v0/things/:id-:ip";
    std::vector<Key> keys;
    PathToRegExpOptions op;
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
    PathToRegExpOptions op;
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
