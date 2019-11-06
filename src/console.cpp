#include "expresscpp/console.hpp"

#include <iostream>

#ifdef EXPRESSCPP_USE_STACKTRACE
#include "boost/stacktrace.hpp"
#endif

#include "expresscpp/date.hpp"

namespace expresscpp {

#if __GNUC__ >= 9 || __clang_major__ >= 9
void Console::PrintMessage(const std::string_view prefix, const std::string_view color, const std::string_view message,
                           const std::experimental::source_location &location) {
  if (message.back() == '\n') {
    std::cout << color << "[" << Date::getTime() << "]" << prefix << location.file_name() << ":" << location.line()
              << "-" << location.function_name() << "()"
              << " -- " << message << kReset;
    return;
  }
  std::cout << color << "[" << Date::getTime() << "]" << prefix << location.file_name() << ":" << location.line() << "-"
            << location.function_name() << "()"
            << " -- " << message << kReset << std::endl;
}

void Console::Log(const std::string_view message, const std::experimental::source_location &location) {
  PrintMessage(" -L- ", kReset, message, location);
}

void Console::Trace(const std::string_view message, const std::experimental::source_location &location) {
  PrintMessage(" -T- ", kBoldcyan, message, location);
#ifdef EXPRESSCPP_USE_STACKTRACE
  std::cerr << boost::stacktrace::stacktrace();
#endif
}

void Console::Error(const std::string_view message, const std::experimental::source_location &location) {
  PrintMessage(" -E- ", kBoldred, message, location);
}

void Console::Debug(const std::string_view message, const std::experimental::source_location &location) {
  PrintMessage(" -D- ", kBlue, message, location);
}

#else

void Console::PrintMessage(const std::string_view prefix, const std::string_view color,
                           const std::string_view message) {
  if (message.back() == '\n') {
    std::cout << color << "[" << Date::getTime() << "]" << prefix << " -- " << message << kReset;
    return;
  }
  std::cout << color << "[" << Date::getTime() << "]" << prefix << " -- " << message << kReset << std::endl;
}

void Console::Log(const std::string_view message) {
  PrintMessage(" -L- ", kReset, message);
}

void Console::Trace(const std::string_view message) {
  PrintMessage(" -T- ", kBoldcyan, message);
#ifdef EXPRESSCPP_USE_STACKTRACE
  std::cerr << boost::stacktrace::stacktrace();
#endif
}

void Console::Error(const std::string_view message) {
  PrintMessage(" -E- ", kBoldred, message);
}

void Console::Debug(const std::string_view message) {
  PrintMessage(" -D- ", kBlue, message);
}

#endif

}  // namespace expresscpp
