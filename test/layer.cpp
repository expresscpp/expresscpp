#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "expresscpp/impl/routing_stack.hpp"
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

using namespace expresscpp;
using namespace std::string_literals;

TEST(LayerTests, TestKeyFirst) {
  auto layer = Layer("/:id");
  auto req = std::make_shared<Request>("/12");
  EXPECT_TRUE(layer.Match(req));
  EXPECT_EQ(layer.GetKeys().size(), 1);
  EXPECT_EQ(layer.GetKeys()[0].name_, "id");
  EXPECT_EQ(layer.GetKeys()[0].index_, 0);
  EXPECT_EQ(req->GetParams().at("id"), "12");
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
  auto req = std::make_shared<Request>("/12/a");
  EXPECT_TRUE(layer.Match(req));
  EXPECT_EQ(layer.GetKeys().size(), 1);
  EXPECT_EQ(layer.GetKeys()[0].name_, "id");
  EXPECT_EQ(layer.GetKeys()[0].index_, 0);
  EXPECT_EQ(req->GetParams().at("id"), "12");
}

TEST(LayerTests, TestWithMultipleKeys) {
  auto layer = Layer("/:id1/a/:id2");
  auto req = std::make_shared<Request>("/12/a/13");
  EXPECT_EQ(layer.GetKeys().size(), 2);
  EXPECT_TRUE(layer.Match(req));
  EXPECT_EQ(req->GetParams().size(), 2);
  EXPECT_EQ(layer.GetKeys()[0].name_, "id1");
  EXPECT_EQ(layer.GetKeys()[0].index_, 0);
  EXPECT_EQ(req->GetParams().at("id1"), "12");
  EXPECT_EQ(layer.GetKeys()[1].name_, "id2");
  EXPECT_EQ(layer.GetKeys()[1].index_, 1);
  EXPECT_EQ(req->GetParams().at("id2"), "13");
}

TEST(LayerTests, TestWithRangesAtEnd) {
  auto layer = Layer("/abc/:from-:to");
  auto req = std::make_shared<Request>("/abc/100-200");
  EXPECT_EQ(layer.GetKeys().size(), 2);
  EXPECT_TRUE(layer.Match(req));
  EXPECT_EQ(req->GetParams().size(), 2);
  EXPECT_EQ(layer.GetKeys()[0].name_, "from");
  EXPECT_EQ(layer.GetKeys()[0].index_, 0);
  EXPECT_EQ(layer.GetKeys()[1].name_, "to");
  EXPECT_EQ(layer.GetKeys()[1].index_, 1);
  EXPECT_EQ(req->GetParams().at("from"), "100");
  EXPECT_EQ(req->GetParams().at("to"), "200");
}

TEST(LayerTests, TestWithRangesInBetween) {
  auto layer = Layer("/abc/:from-:to/a");
  EXPECT_EQ(layer.GetKeys().size(), 2);
  auto req = std::make_shared<Request>("/abc/100-200/a");
  EXPECT_TRUE(layer.Match(req));
  EXPECT_EQ(req->GetParams().size(), 2);
  EXPECT_EQ(layer.GetKeys()[0].name_, "from");
  EXPECT_EQ(layer.GetKeys()[0].index_, 0);
  EXPECT_EQ(layer.GetKeys()[1].name_, "to");
  EXPECT_EQ(layer.GetKeys()[1].index_, 1);
  EXPECT_EQ(req->GetParams().at("from"), "100");
  EXPECT_EQ(req->GetParams().at("to"), "200");
}

TEST(LayerTests, TestWithMultipleRanges) {
  auto layer = Layer("/abc/:from1-:to1/a/:from2-:to2/:from3-:to3/test/:from4-:to4/:from5-:to5");
  EXPECT_EQ(layer.GetKeys().size(), 10);
  auto req = std::make_shared<Request>("/abc/1-2/a/3-4/5-6/test/7-8/9-10");
  EXPECT_TRUE(layer.Match(req));
  EXPECT_EQ(req->GetParams().size(), 10);
  size_t name_index = 1;
  for (size_t index = 0; index < req->GetParams().size();) {
    const auto from = "from" + std::to_string(name_index);
    const auto to = "to" + std::to_string(name_index);
    EXPECT_EQ(layer.GetKeys()[index].name_, from);
    EXPECT_EQ(layer.GetKeys()[index].index_, index);
    EXPECT_EQ(layer.GetKeys()[index + 1].name_, to);
    EXPECT_EQ(layer.GetKeys()[index + 1].index_, index + 1);
    EXPECT_EQ(req->GetParams().at(from), std::to_string(index + 1));
    EXPECT_EQ(req->GetParams().at(to), std::to_string(index + 2));
    index += 2;
    ++name_index;
  }
}

TEST(LayerTests, TestWithManyKeys) {
  auto layer = Layer("/:id1/a/:id2/:id3/:id4/:id5/x/y/z/:id6");
  EXPECT_EQ(layer.GetKeys().size(), 6);
  auto req = std::make_shared<Request>("/1/a/2/3/4/5/x/y/z/6");
  EXPECT_TRUE(layer.Match(req));
  EXPECT_EQ(req->GetParams().size(), 6);
  for (size_t index = 0; index < 6; ++index) {
    const auto name = "id" + std::to_string(index + 1);
    EXPECT_EQ(layer.GetKeys()[index].name_, name);
    EXPECT_EQ(layer.GetKeys()[index].index_, index);
    EXPECT_EQ(req->GetParams().at(name), std::to_string(index + 1));
  }
}

TEST(LayerTests, TestNonMatching) {
  auto layer = Layer("/:id1/a/:id2/:id3/:id4/:id5/x/y/z/:id6");
  auto req = std::make_shared<Request>("/1/abc/2/3/4/5/x/y/z/6");
  EXPECT_FALSE(layer.Match(req));
  EXPECT_EQ(req->GetParams().size(), 0);
}

TEST(LayerTests, TestMultiMatchings) {
  auto layer = Layer("/:id1/a/:id2/:id3/:id4/:id5/x/y/z/:id6");
  EXPECT_EQ(layer.GetKeys().size(), 6);
  auto req = std::make_shared<Request>("/1/a/2/3/4/5/x/y/z/6");
  EXPECT_TRUE(layer.Match(req));
  EXPECT_EQ(req->GetParams().size(), 6);
  auto req2 = std::make_shared<Request>("/1/abc/2/3/4/5/x/y/z/6");
  EXPECT_FALSE(layer.Match(req2));
  EXPECT_EQ(req2->GetParams().size(), 0);

  auto req3 = std::make_shared<Request>("/1/a/2/3/4/5/x/y/z/6");
  EXPECT_TRUE(layer.Match(req3));
  EXPECT_EQ(layer.GetKeys().size(), 6);
  for (size_t index = 0; index < 6; ++index) {
    const auto name = "id" + std::to_string(index + 1);
    EXPECT_EQ(layer.GetKeys()[index].name_, name);
    EXPECT_EQ(layer.GetKeys()[index].index_, index);
    EXPECT_EQ(req3->GetParams().at(name), std::to_string(index + 1));
  }
}

TEST(LayerTests, TestQueryParams) {
  auto layer = Layer("/abc");
  auto req = std::make_shared<Request>("/abc?key=value");
  EXPECT_TRUE(layer.Match(req));
  EXPECT_EQ(req->GetQueryParams().size(), 1);
}

TEST(LayerTests, TestWithMoreQueryParams) {
  auto layer = Layer("/abc");
  auto req = std::make_shared<Request>("/abc?key1=value1&key2=value2&key3=value3");
  EXPECT_TRUE(layer.Match(req));
  EXPECT_EQ(req->GetQueryParams().size(), 3);
}

TEST(LayerTests, TestWithParamsAndQueryParams) {
  auto layer = Layer("/abc/:id");
  auto req = std::make_shared<Request>("/abc/123?key1=value1&key2=value2&key3=value3");
  EXPECT_TRUE(layer.Match(req));
  EXPECT_EQ(req->GetQueryParams().size(), 3);
  EXPECT_EQ(req->GetParams().size(), 1);
  for (size_t index = 0; index < req->GetQueryParams().size(); ++index) {
    const auto key = "key" + std::to_string(index + 1);
    const auto value = "value" + std::to_string(index + 1);
    EXPECT_EQ(req->GetQueryParams().count(key), 1);
    EXPECT_EQ(req->GetQueryParams().at(key), value);
  }
}

TEST(LayerTests, TestWithInvalidQueryParams) {
  auto layer = Layer("/abc");
  auto req = std::make_shared<Request>("/abc?");
  EXPECT_TRUE(layer.Match(req));
  EXPECT_EQ(req->GetQueryParams().size(), 0);
  EXPECT_EQ(req->GetParams().size(), 0);
  auto req1 = std::make_shared<Request>("/abc??");
  EXPECT_TRUE(layer.Match(req1));
  EXPECT_EQ(req1->GetQueryParams().size(), 0);
  EXPECT_EQ(req1->GetParams().size(), 0);
  auto req2 = std::make_shared<Request>("/abc&&");
  EXPECT_FALSE(layer.Match(req2));
  EXPECT_EQ(req2->GetQueryParams().size(), 0);
  EXPECT_EQ(req2->GetParams().size(), 0);
  auto req3 = std::make_shared<Request>("/abc?&&&&&??");
  EXPECT_TRUE(layer.Match(req3));
  EXPECT_EQ(req3->GetQueryParams().size(), 0);
  EXPECT_EQ(req3->GetParams().size(), 0);
}

TEST(LayerTests, TestWithIncompleteQueryParams) {
  auto layer = Layer("/abc/:id");
  auto req = std::make_shared<Request>("/abc/123?key1=value1&key2&key3=value3");
  EXPECT_TRUE(layer.Match(req));
  EXPECT_EQ(req->GetQueryParams().size(), 1);
  for (size_t index = 0; index < req->GetQueryParams().size(); ++index) {
    const auto key = "key" + std::to_string(index + 1);
    const auto value = "value" + std::to_string(index + 1);
    EXPECT_EQ(req->GetQueryParams().count(key), 1);
    EXPECT_EQ(req->GetQueryParams().at(key), value);
  }
}
