#pragma once

#include <filesystem>
#include <string>

#include "expresscpp/types.hpp"

namespace expresscpp {

class StaticFileProvider {
 public:
  StaticFileProvider(std::filesystem::path path_to_root_folder);

  void UsePrefix(std::string_view path);

  void HandleRequests(express_request_t req, express_response_t res);

 private:
  std::filesystem::path path_to_root_folder_{std::filesystem::temp_directory_path()};
};

typedef std::shared_ptr<StaticFileProvider> StaticFileProviderPtr;

}  // namespace expresscpp
