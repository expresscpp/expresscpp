#pragma once

#include <string>

#include "expresscpp/exports.hpp"

namespace expresscpp {

struct EXPRESSCPP_API Key {
  std::string name_;
  bool optional_;
  std::size_t offset_;
  std::size_t index_;
};

}  // namespace expresscpp
