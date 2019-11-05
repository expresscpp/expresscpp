#include "expresscpp/console.hpp"

#include <iostream>

#ifdef EXPRESSCPP_USE_STACKTRACE
#include "boost/stacktrace.hpp"
#endif

#include "expresscpp/date.hpp"

#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

namespace expresscpp {

static std::string getFileName(const std::string &s) {
  char sep = '/';

#ifdef _WIN32
  sep = '\\';
#endif

  size_t i = s.rfind(sep, s.length());
  if (i != std::string::npos) {
    return (s.substr(i + 1, s.length() - i));
  }

  return ("");
}

void Console::Log(const std::string_view message, const std::experimental::source_location &location) {
  if (message.back() == '\n') {
    std::cout << RESET << "[" << Date().getTime() << "]"
              << " -I- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
              << " -- " << message;
    return;
  }
  std::cout << RESET << "[" << Date().getTime() << "]"
            << " -I- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
            << " -- " << message << std::endl;
}

void Console::Trace(const std::string_view message, const std::experimental::source_location &location) {
  if (message.back() == '\n') {
    std::cerr << BOLDCYAN << "[" << Date().getTime() << "]"
              << " -T- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
              << " -- " << message << RESET;
    return;
  }
  std::cerr << BOLDCYAN << "[" << Date().getTime() << "]"
            << " -T- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
            << " -- " << message << RESET << std::endl;
#ifdef EXPRESSCPP_USE_STACKTRACE
  // TODO: this does not seem to be working properly:
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
    std::cerr << BOLDRED << "[" << Date().getTime() << "]"
              << " -E- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
              << " -- " << message << RESET;
    return;
  }
  std::cerr << BOLDRED << "[" << Date().getTime() << "]"
            << " -E- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
            << " -- " << message << RESET << std::endl;
}

void Console::Debug(const std::string_view message, const std::experimental::source_location &location) {
  if (message.back() == '\n') {
    std::cout << RESET << "[" << Date().getTime() << "]"
              << " -D- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
              << " -- " << message;
    return;
  }
  std::cout << RESET << "[" << Date().getTime() << "]"
            << " -D- " << location.file_name() << ":" << location.line() << "-" << location.function_name() << "()"
            << " -- " << message << std::endl;
}

}  // namespace expresscpp
