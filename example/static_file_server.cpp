#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"

using namespace expresscpp;

int main() {
  ExpressCpp expresscpp;

  Console::setLogLevel(LogLevel::kDebug);

  // get a folder to write to
  const auto uuid_ = boost::uuids::random_generator()();
  const auto tmp_folder = std::filesystem::temp_directory_path();
  const std::string doc_root = tmp_folder.string() + "/www" + boostUUIDToString(uuid_);
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

  {
    std::filesystem::path path_to_index_html = doc_root + "/index.html";
    std::ofstream index_html_file(path_to_index_html);
    index_html_file << index_html_content << std::endl;
    index_html_file.close();
  }

  // tell express to look for files in this directory
  expresscpp.Use(expresscpp.GetStaticFileProvider(doc_root));

  // start listening for requests and block until ctrl+C
  const uint16_t port = 8081u;
  expresscpp
      .Listen(port,
              [&](auto ec) {
                if (ec) {
                  std::cerr << "ERROR: " << ec.message() << std::endl;
                  exit(1);
                }
                std::cout << "Example app listening on port " << port << std::endl;
                std::cout << "go to http://localhost:" << port << std::endl;
              })
      .Block();

  return 0;
}
