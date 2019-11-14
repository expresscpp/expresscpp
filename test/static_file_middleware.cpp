#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "expresscpp/expresscpp.hpp"
#include "expresscpp/fetch.hpp"
#include "gtest/gtest.h"

using namespace expresscpp;

constexpr uint16_t port = 8081u;

TEST(StaticFileMiddleware, ServeIndexHtml) {
  auto expresscpp = std::make_shared<ExpressCpp>();

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
  std::filesystem::path path_to_index_html = "/tmp/index.html";
  std::ofstream index_html_file(path_to_index_html);
  index_html_file << index_html_content << std::endl;
  index_html_file.close();
  assert(std::filesystem::exists(path_to_index_html));
  auto parent = path_to_index_html.parent_path();
  expresscpp->Use(expresscpp->GetStaticFileProvider(parent));

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
