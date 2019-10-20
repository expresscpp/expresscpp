#pragma once

#include <filesystem>
#include <iostream>

#include "expresscpp/types.hpp"

class StaticFileProvider {
 public:
  StaticFileProvider(std::filesystem::path path_to_root_folder);

  void UsePrefix(std::string_view path);

  void HandleRequests(express_request_t req, express_response_t res);

 private:
  std::filesystem::path path_to_root_folder_{"/tmp"};
  std::string_view doc_root{"/tmp"};
};

typedef std::shared_ptr<StaticFileProvider> StaticFileProviderPtr;
