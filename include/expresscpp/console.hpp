#pragma once

// TODO(gocarlos): remove this again as soon as we do not use gcc 8 anymore
#if __GNUC__ >= 9 || __clang_major__ >= 9
#include <experimental/source_location>
#define EXPRESSCPP_CURRENT_SOURCE_LOCATION \
  , const std::experimental::source_location& location = std::experimental::source_location::current()

#define EXPRESSCPP_SOURCE_LOCATION , const std::experimental::source_location& location

#else
#define EXPRESSCPP_CURRENT_SOURCE_LOCATION
#define EXPRESSCPP_SOURCE_LOCATION
#endif

#include <iostream>
#include <mutex>
#include <string_view>

#include "fmt/format.h"

namespace expresscpp {

enum class LogLevel {
  kDebug,
  kError,
};

class Console {
 public:
  //! @brief usageConsole::Log(fmt::format("my int: {}", 2));
  static void Log(const std::string_view message EXPRESSCPP_CURRENT_SOURCE_LOCATION);
  static void Trace(const std::string_view message EXPRESSCPP_CURRENT_SOURCE_LOCATION);
  static void Error(const std::string_view message EXPRESSCPP_CURRENT_SOURCE_LOCATION);
  static void Debug(const std::string_view message EXPRESSCPP_CURRENT_SOURCE_LOCATION);

  static void setLogLevel(const LogLevel& log_level);

 private:
  static LogLevel log_level_;

  static void PrintMessage(const std::string_view prefix, const std::string_view color,
                           const std::string_view message EXPRESSCPP_SOURCE_LOCATION);

  static constexpr std::string_view kReset = "\033[0m";
  static constexpr std::string_view kBlack = "\033[30m";              /* Black */
  static constexpr std::string_view kRed = "\033[31m";                /* Red */
  static constexpr std::string_view kGreen = "\033[32m";              /* Green */
  static constexpr std::string_view kYellow = "\033[33m";             /* Yellow */
  static constexpr std::string_view kBlue = "\033[34m";               /* Blue */
  static constexpr std::string_view kMagenta = "\033[35m";            /* Magenta */
  static constexpr std::string_view kCyan = "\033[36m";               /* Cyan */
  static constexpr std::string_view kWhite = "\033[37m";              /* White */
  static constexpr std::string_view kBoldblack = "\033[1m\033[30m";   /* Bold Black */
  static constexpr std::string_view kBoldred = "\033[1m\033[31m";     /* Bold Red */
  static constexpr std::string_view kBoldgreen = "\033[1m\033[32m";   /* Bold Green */
  static constexpr std::string_view kBoldyellow = "\033[1m\033[33m";  /* Bold Yellow */
  static constexpr std::string_view kBoldblue = "\033[1m\033[34m";    /* Bold Blue */
  static constexpr std::string_view kBoldmagenta = "\033[1m\033[35m"; /* Bold Magenta */
  static constexpr std::string_view kBoldcyan = "\033[1m\033[36m";    /* Bold Cyan */
  static constexpr std::string_view kBoldwhite = "\033[1m\033[37m";   /* Bold White */

  static std::mutex mutex_;
};

}  // namespace expresscpp
