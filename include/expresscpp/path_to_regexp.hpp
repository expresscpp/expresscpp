#pragma once

// https://github.com/pillarjs/path-to-regexp

#include <string_view>
#include <vector>

#include "expresscpp/key.hpp"

namespace expresscpp {

struct PathToRegExpOptions {
  bool sensitive{false};
  bool strict{false};
  bool end{true};
};

/*!
 * \brief pathtoRegexp parses path and creates vector of keys dependent on the options
 *  path = "/api/v0/things/:id/:ip" -> keys = ["id", "ip"];
 * \param path which is registered by the user, example: "/api/v0/things/:id/:ip"
 * \param keys vector of found keys in the path example: ["id", "ip"]
 * \param op TODO(gocarlos): implement me
 * \return
 */
std::string pathToRegExpString(std::string_view path, std::vector<Key>& keys, PathToRegExpOptions op);

}  // namespace expresscpp
