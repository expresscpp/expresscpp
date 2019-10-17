#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "expresscpp/expresscpp.hpp"

static std::string doc_root = "/home/gocarlos/git/expresscpp/expresscpp/test/test_doc_root";

int main() {
  std::cout << "Hello World!" << std::endl;

  auto expresscpp = std::make_shared<ExpressCpp>();

  expresscpp->Use(expresscpp->GetStaticFileProvider(doc_root));

  const uint16_t port = 8080u;
  expresscpp->Listen(port, []() { std::cout << "Example app listening on port " << port << std::endl; }).Block();

  return 0;
}
