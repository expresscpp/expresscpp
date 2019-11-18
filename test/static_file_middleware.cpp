#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "expresscpp/middleware/serve_static_provider.hpp"
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

using namespace expresscpp;

constexpr uint16_t port = 8081u;

TEST(StaticFileMiddleware, ServeIndexHtml) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  // create temp folder for html files
  const auto uuid_ = boost::uuids::random_generator()();
  const auto tmp_folder = std::filesystem::temp_directory_path();
  const std::string doc_root = tmp_folder.string() + "/www-" + boostUUIDToString(uuid_);
  std::filesystem::create_directory(doc_root);

  std::string index_html_content =
      R"(<!doctype html>
<html>
  <head>
    <title>This is the title of the webpage!</title>
  </head>
  <body>
    <p>This is a paragraph.</p>
  </body>
</html>
)";

  std::string index_doc_content = R"({"status": "ok"})";

  // create html files
  {
    std::filesystem::path path_to_index_html = doc_root + "/index.html";
    std::ofstream index_html_file(path_to_index_html);
    index_html_file << index_html_content << std::endl;
    index_html_file.close();
    assert(std::filesystem::exists(path_to_index_html));
  }

  // create other doc files such as json
  {
    std::filesystem::path path_to_doc = doc_root + "/doc.json";
    std::ofstream index_doc_file(path_to_doc);
    index_doc_file << index_doc_content << std::endl;
    index_doc_file.close();
    assert(std::filesystem::exists(path_to_doc));
  }

  StaticFileProvider static_file_provider(doc_root);

  expresscpp->Use(static_file_provider);

  expresscpp->Listen(port, [=](auto ec) {
    EXPECT_FALSE(ec);

    {
      // should get the index.html file
      auto index_html_contect_response = fetch(fmt::format("localhost:{}/", port), {.method = HttpMethod::Get});

      index_html_contect_response.erase(
          std::remove(index_html_contect_response.begin(), index_html_contect_response.end(), '\n'),
          index_html_contect_response.end());
      std::string index_html_content_expected = index_html_content;
      index_html_content_expected.erase(
          std::remove(index_html_content_expected.begin(), index_html_content_expected.end(), '\n'),
          index_html_content_expected.end());

      EXPECT_EQ(index_html_contect_response, index_html_content_expected);
    }

    {
      // should get the doc.json file
      auto json_content_response = fetch(fmt::format("localhost:{}/doc.json", port));
      const auto result = nlohmann::json::parse(json_content_response);
      const auto expected = nlohmann::json::parse(index_doc_content);
      EXPECT_EQ(expected["status"], expected["status"]);
    }
  });
}
