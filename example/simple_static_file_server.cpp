#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "expresscpp/expresscpp.hpp"

int main() {
  std::cout << "Hello World!" << std::endl;

  auto expresscpp = std::make_shared<ExpressCpp>();

  // get a folder to write to
  std::string doc_root = "/tmp/www";
  std::filesystem::create_directory(doc_root);

  // create a html index file
  std::string index_html_content = R"(
  <!doctype html>
  <html>
    <head>
      <title>This is the title of the webpage!</title>
    </head>
    <body>
      <p>This is a paragraph.</p>
    </body>
  </html>
  )";
  std::filesystem::path path_to_index_html = doc_root + "/index.html";
  std::ofstream index_html_file(path_to_index_html);
  index_html_file << index_html_content << std::endl;
  index_html_file.close();

  // tell express to look for files in this directory
  expresscpp->Use(expresscpp->GetStaticFileProvider(doc_root));

  // start listening for requests and block until ctrl+C
  const uint16_t port = 8080u;
  expresscpp->Listen(port, []() { std::cout << "Example app listening on port " << port << std::endl; }).Block();

  return 0;
}
