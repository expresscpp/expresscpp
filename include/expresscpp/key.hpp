#pragma once

#include <string>

namespace expresscpp {

struct Key {
  std::string name_;
  bool optional_;
  std::size_t offset_;
};

}  // namespace expresscpp
