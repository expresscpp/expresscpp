#include "expresscpp/path_to_regexp.hpp"

#include <algorithm>
#include <stdexcept>

#define EXPRESSCPP_CONFIG_DEBUG_PATH_TO_REGEX 0

namespace expresscpp {
using namespace std::string_literals;

std::string pathToRegExpString(std::string_view registered_path, std::vector<Key>& keys, PathToRegExpOptions op,
                               std::string_view parent_path) {
  std::string regex = std::string("^") + parent_path.data() + registered_path.data();
  size_t key_index = 0;
  std::string::size_type start_pos = 0;
  while (start_pos != std::string::npos) {
    start_pos = regex.find(":", start_pos);
    if (start_pos != std::string::npos) {
      const auto dash_end_pos = regex.find("-", start_pos);
      const auto slash_end_pos = regex.find("/", start_pos);
      const auto end_pos = std::min(dash_end_pos, slash_end_pos);
      Key tmp_key;
      const auto key_name = regex.substr(start_pos + 1, end_pos - start_pos - 1);
      if (key_name.empty()) {
        throw std::runtime_error(std::string("Empty parameter name found in path: ") + registered_path.data());
      }
      tmp_key.index_ = key_index++;
      tmp_key.name_ = std::move(key_name);
      regex.replace(start_pos, end_pos - start_pos, "(\\S+)");
      keys.emplace_back(tmp_key);
    }
  }
  if (op.end) {
    regex += "$";
  } else if (op.strict) {
    regex += "/\\S*";
  } else {
    regex += "\\S*";
  }

#if EXPRESSCPP_CONFIG_DEBUG_PATH_TO_REGEX
  if (keys.size() > 0) {
    std::cout << "printing keys ***********************" << std::endl;
    for (const auto& k : keys) {
      std::cout << "key found : "
                << "\"" << k.name_ << "\"" << std::endl;
    }
    std::cout << "printing keys **********************" << std::endl;
  }
#endif

  return regex;
}
}  // namespace expresscpp
