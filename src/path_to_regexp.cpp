#include "expresscpp/path_to_regexp.hpp"

#define EXPRESSCPP_CONFIG_DEBUG_PATH_TO_REGEX 0

namespace expresscpp {
using namespace std::string_literals;

std::string pathToRegExpString(std::string_view path, std::vector<Key>& keys, PathToRegExpOptions op) {
  std::string path_string = path.data();
  std::string regex;
  //  if (regex == "") {
  //    regex = "/";
  //  }

  regex = ("^" + path_string + (op.strict ? "" : path_string.back() == '/' ? "?"s : "/?"s));

  regex += (op.end ? "$" : (path_string.back() == '/' ? "" : "(?=\\/|$)"));

  // TODO(gocarlos): finish this.
  //  fill in the keys -> "/api/v0/things/:id/:ip" -> ["id", "ip"];
  Key key = {.name_ = "id", .optional_ = false, .offset_ = 0};
  keys.push_back(key);

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
