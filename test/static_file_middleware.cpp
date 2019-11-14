#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "gtest/gtest.h"

using namespace expresscpp;

constexpr uint16_t port = 8081u;

TEST(StaticFileMiddleware, ServeIndexHtml) {
  auto expresscpp = std::make_shared<ExpressCpp>();

  const auto uuid_ = boost::uuids::random_generator()();
  const auto tmp_folder = std::filesystem::temp_directory_path();
  const std::string doc_root = tmp_folder.string() + "/www" + boostUUIDToString(uuid_);
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

  {
    std::filesystem::path path_to_index_html = doc_root + "/index.html";
    std::ofstream index_html_file(path_to_index_html);
    index_html_file << index_html_content << std::endl;
    index_html_file.close();
    assert(std::filesystem::exists(path_to_index_html));
  }

  expresscpp->Use(expresscpp->GetStaticFileProvider(doc_root));

  expresscpp->Listen(port, [=](auto ec) {
    EXPECT_FALSE(ec);

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
  });
}
