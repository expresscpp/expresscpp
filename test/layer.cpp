#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "expresscpp/impl/routing_stack.hpp"
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

using namespace expresscpp;
using namespace std::string_literals;

TEST(LayerTests, TestKeyFirst) {
  auto layer = Layer("/:id");
  EXPECT_TRUE(layer.match("/12"));
  EXPECT_EQ(layer.keys_.size(), 1);
  EXPECT_EQ(layer.keys_[0].name_, "id");
  EXPECT_EQ(layer.keys_[0].index_, 0);
  EXPECT_EQ(layer.params_["id"], "12");
}

TEST(LayerTests, TestIncompleteKey) {
  EXPECT_ANY_THROW(Layer(":"));
  EXPECT_ANY_THROW(Layer("/:"));
  EXPECT_ANY_THROW(Layer("/a/b/:/c"));
  EXPECT_ANY_THROW(Layer("/a/b/c/:"));
  EXPECT_ANY_THROW(Layer("/a/:/c/:"));
}

TEST(LayerTests, TestWithSingleKey) {
  auto layer = Layer("/:id/a");
  EXPECT_TRUE(layer.match("/12/a"));
  EXPECT_EQ(layer.keys_.size(), 1);
  EXPECT_EQ(layer.keys_[0].name_, "id");
  EXPECT_EQ(layer.keys_[0].index_, 0);
  EXPECT_EQ(layer.params_["id"], "12");
}

TEST(LayerTests, TestWithMultipleKeys) {
  auto layer = Layer("/:id1/a/:id2");
  EXPECT_EQ(layer.keys_.size(), 2);
  EXPECT_TRUE(layer.match("/12/a/13"));
  EXPECT_EQ(layer.params_.size(), 2);
  EXPECT_EQ(layer.keys_[0].name_, "id1");
  EXPECT_EQ(layer.keys_[0].index_, 0);
  EXPECT_EQ(layer.params_["id1"], "12");
  EXPECT_EQ(layer.keys_[1].name_, "id2");
  EXPECT_EQ(layer.keys_[1].index_, 1);
  EXPECT_EQ(layer.params_["id2"], "13");
}

TEST(LayerTests, TestWithRangesAtEnd) {
  auto layer = Layer("/abc/:from-:to");
  EXPECT_EQ(layer.keys_.size(), 2);
  EXPECT_TRUE(layer.match("/abc/100-200"));
  EXPECT_EQ(layer.params_.size(), 2);
  EXPECT_EQ(layer.keys_[0].name_, "from");
  EXPECT_EQ(layer.keys_[0].index_, 0);
  EXPECT_EQ(layer.keys_[1].name_, "to");
  EXPECT_EQ(layer.keys_[1].index_, 1);
  EXPECT_EQ(layer.params_["from"], "100");
  EXPECT_EQ(layer.params_["to"], "200");
}

TEST(LayerTests, TestWithRangesInBetween) {
  auto layer = Layer("/abc/:from-:to/a");
  EXPECT_EQ(layer.keys_.size(), 2);
  EXPECT_TRUE(layer.match("/abc/100-200/a"));
  EXPECT_EQ(layer.params_.size(), 2);
  EXPECT_EQ(layer.keys_[0].name_, "from");
  EXPECT_EQ(layer.keys_[0].index_, 0);
  EXPECT_EQ(layer.keys_[1].name_, "to");
  EXPECT_EQ(layer.keys_[1].index_, 1);
  EXPECT_EQ(layer.params_["from"], "100");
  EXPECT_EQ(layer.params_["to"], "200");
}

TEST(LayerTests, TestWithMultipleRanges) {
  auto layer = Layer("/abc/:from1-:to1/a/:from2-:to2/:from3-:to3/test/:from4-:to4/:from5-:to5");
  EXPECT_EQ(layer.keys_.size(), 10);
  EXPECT_TRUE(layer.match("/abc/1-2/a/3-4/5-6/test/7-8/9-10"));
  EXPECT_EQ(layer.params_.size(), 10);
  size_t name_index = 1;
  for (size_t index = 0; index < layer.params_.size();) {
    const auto from = "from" + std::to_string(name_index);
    const auto to = "to" + std::to_string(name_index);
    EXPECT_EQ(layer.keys_[index].name_, from);
    EXPECT_EQ(layer.keys_[index].index_, index);
    EXPECT_EQ(layer.keys_[index + 1].name_, to);
    EXPECT_EQ(layer.keys_[index + 1].index_, index + 1);
    EXPECT_EQ(layer.params_[from], std::to_string(index + 1));
    EXPECT_EQ(layer.params_[to], std::to_string(index + 2));
    index += 2;
    ++name_index;
  }
}

TEST(LayerTests, TestWithManyKeys) {
  auto layer = Layer("/:id1/a/:id2/:id3/:id4/:id5/x/y/z/:id6");
  EXPECT_EQ(layer.keys_.size(), 6);
  EXPECT_TRUE(layer.match("/1/a/2/3/4/5/x/y/z/6"));
  EXPECT_EQ(layer.params_.size(), 6);
  for (size_t index = 0; index < 6; ++index) {
    const auto name = "id" + std::to_string(index + 1);
    EXPECT_EQ(layer.keys_[index].name_, name);
    EXPECT_EQ(layer.keys_[index].index_, index);
    EXPECT_EQ(layer.params_[name], std::to_string(index + 1));
  }
}

TEST(LayerTests, TestNonMatching) {
  auto layer = Layer("/:id1/a/:id2/:id3/:id4/:id5/x/y/z/:id6");
  EXPECT_FALSE(layer.match("/1/abc/2/3/4/5/x/y/z/6"));
  EXPECT_EQ(layer.params_.size(), 0);
}

TEST(LayerTests, TestMultiMatchings) {
  auto layer = Layer("/:id1/a/:id2/:id3/:id4/:id5/x/y/z/:id6");
  EXPECT_EQ(layer.keys_.size(), 6);
  EXPECT_TRUE(layer.match("/1/a/2/3/4/5/x/y/z/6"));
  EXPECT_EQ(layer.params_.size(), 6);
  EXPECT_FALSE(layer.match("/1/abc/2/3/4/5/x/y/z/6"));
  EXPECT_EQ(layer.params_.size(), 0);

  EXPECT_TRUE(layer.match("/1/a/2/3/4/5/x/y/z/6"));
  EXPECT_EQ(layer.keys_.size(), 6);
  for (size_t index = 0; index < 6; ++index) {
    const auto name = "id" + std::to_string(index + 1);
    EXPECT_EQ(layer.keys_[index].name_, name);
    EXPECT_EQ(layer.keys_[index].index_, index);
    EXPECT_EQ(layer.params_[name], std::to_string(index + 1));
  }
}

TEST(LayerTests, TestQueryParams) {
  auto layer = Layer("/abc");
  EXPECT_TRUE(layer.match("/abc?key=value"));
  EXPECT_EQ(layer.query_params_.size(), 1);
}

TEST(LayerTests, TestWithMoreQueryParams) {
  auto layer = Layer("/abc");
  EXPECT_TRUE(layer.match("/abc?key1=value1&key2=value2&key3=value3"));
  EXPECT_EQ(layer.query_params_.size(), 3);
}

TEST(LayerTests, TestWithParamsAndQueryParams) {
  auto layer = Layer("/abc/:id");
  EXPECT_TRUE(layer.match("/abc/123?key1=value1&key2=value2&key3=value3"));
  EXPECT_EQ(layer.query_params_.size(), 3);
  EXPECT_EQ(layer.params_.size(), 1);
  for (size_t index = 0; index < layer.query_params_.size(); ++index) {
    const auto key = "key" + std::to_string(index + 1);
    const auto value = "value" + std::to_string(index + 1);
    EXPECT_EQ(layer.query_params_.count(key), 1);
    EXPECT_EQ(layer.query_params_[key], value);
  }
}

TEST(LayerTests, TestWithInvalidQueryParams) {
  auto layer = Layer("/abc");
  EXPECT_TRUE(layer.match("/abc?"));
  EXPECT_EQ(layer.query_params_.size(), 0);
  EXPECT_EQ(layer.params_.size(), 0);
  EXPECT_TRUE(layer.match("/abc??"));
  EXPECT_EQ(layer.query_params_.size(), 0);
  EXPECT_EQ(layer.params_.size(), 0);
  EXPECT_FALSE(layer.match("/abc&&"));
  EXPECT_EQ(layer.query_params_.size(), 0);
  EXPECT_EQ(layer.params_.size(), 0);
  EXPECT_TRUE(layer.match("/abc?&&&&&??"));
  EXPECT_EQ(layer.query_params_.size(), 0);
  EXPECT_EQ(layer.params_.size(), 0);
}

TEST(LayerTests, TestWithIncompleteQueryParams) {
  auto layer = Layer("/abc/:id");
  EXPECT_TRUE(layer.match("/abc/123?key1=value1&key2&key3=value3"));
  EXPECT_EQ(layer.query_params_.size(), 1);
  for (size_t index = 0; index < layer.query_params_.size(); ++index) {
    const auto key = "key" + std::to_string(index + 1);
    const auto value = "value" + std::to_string(index + 1);
    EXPECT_EQ(layer.query_params_.count(key), 1);
    EXPECT_EQ(layer.query_params_[key], value);
  }
}
