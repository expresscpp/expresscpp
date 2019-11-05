#pragma once

#include <experimental/source_location>
#include <iostream>
#include <string_view>

#include "fmt/format.h"

namespace expresscpp {

class Console {
 public:
  //! @brief usageConsole::Log(fmt::format("my int: {}", 2));
  static void Log(const std::string_view message,
                  const std::experimental::source_location& location = std::experimental::source_location::current());
  static void Trace(const std::string_view message,
                    const std::experimental::source_location& location = std::experimental::source_location::current());
  static void Error(const std::string_view message,
                    const std::experimental::source_location& location = std::experimental::source_location::current());
  static void Debug(const std::string_view message,
                    const std::experimental::source_location& location = std::experimental::source_location::current());
};

}  // namespace expresscpp
