#include "expresscpp/console.hpp"

#include <iostream>

#ifdef EXPRESSCPP_USE_STACKTRACE
#include "boost/stacktrace.hpp"
#endif

#include "expresscpp/date.hpp"

namespace expresscpp {

void Console::Log(const std::string_view message, const std::experimental::source_location &location) {
  if (message.back() == '\n') {
    std::cout << kReset << "[" << Date::getTime() << "]"
              << " -I- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
              << " -- " << message;
    return;
  }
  std::cout << kReset << "[" << Date::getTime() << "]"
            << " -I- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
            << " -- " << message << std::endl;
}

void Console::Trace(const std::string_view message, const std::experimental::source_location &location) {
  if (message.back() == '\n') {
    std::cerr << kBoldcyan << "[" << Date::getTime() << "]"
              << " -T- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
              << " -- " << message << kReset;
    return;
  }
  std::cerr << kBoldcyan << "[" << Date::getTime() << "]"
            << " -T- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
            << " -- " << message << kReset << std::endl;
#ifdef EXPRESSCPP_USE_STACKTRACE
  // TODO(gocarlos): this does not seem to be working properly:
  /*
clang-format off
 0# 0x000055555575D4CC in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
 1# 0x00005555558759F7 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
 2# 0x000055555576D157 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
 3# 0x000055555577FD5F in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
 4# 0x000055555577FA90 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
 5# 0x000055555577F844 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
 6# 0x000055555577F50D in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
 7# 0x000055555577F3BC in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
 8# 0x000055555577F219 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
 9# 0x000055555577EE4C in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
10# 0x0000555555771AD3 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
11# 0x0000555555771C72 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
12# 0x0000555555783D2B in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
13# 0x0000555555782B54 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
14# 0x000055555578410F in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
15# 0x00005555556F346E in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
16# 0x00005555557859D7 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
17# 0x0000555555784F48 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
18# 0x00005555557843BF in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
19# 0x0000555555782EB4 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
20# 0x0000555555781F0D in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
21# 0x00005555557815EE in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
22# 0x0000555555780B19 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
23# 0x000055555577DEEB in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
24# 0x000055555577D60F in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
25# 0x000055555577CB3B in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
26# 0x000055555577BDE0 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
27# 0x00005555556F346E in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
28# 0x00005555556F77E2 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
29# 0x00005555556F70AC in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
30# 0x00005555556F7D80 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
31# 0x00005555556EA547 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
32# 0x00005555556F15D3 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
33# 0x00005555556F0DD8 in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
34# 0x00005555556F275E in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
35# 0x00005555556F266C in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
36# 0x00005555556F25EE in /home/ladmin/git/expresscpp/build-expresscpp-Desktop-Debug/bin/expresscpp_simple_rest_router_server
clang-format on
*/
  std::cerr << boost::stacktrace::stacktrace();
#endif
}

void Console::Error(const std::string_view message, const std::experimental::source_location &location) {
  if (message.back() == '\n') {
    std::cerr << kBoldred << "[" << Date::getTime() << "]"
              << " -E- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
              << " -- " << message << kReset;
    return;
  }
  std::cerr << kBoldred << "[" << Date::getTime() << "]"
            << " -E- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
            << " -- " << message << kReset << std::endl;
}

void Console::Debug(const std::string_view message, const std::experimental::source_location &location) {
  if (message.back() == '\n') {
    std::cout << kReset << "[" << Date::getTime() << "]"
              << " -D- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
              << " -- " << message;
    return;
  }
  std::cout << kReset << "[" << Date::getTime() << "]"
            << " -D- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
            << " -- " << message << std::endl;
}

}  // namespace expresscpp
