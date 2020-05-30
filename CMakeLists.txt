cmake_minimum_required(VERSION 3.8)

project(expresscpp LANGUAGES CXX)

file(
  STRINGS 
  package.json  
  EXPRESSCPP_PROJECT_VERSION 
  REGEX "version\"\: ")

if(NOT EXPRESSCPP_PROJECT_VERSION)
  message(FATAL_ERROR "Cannot find version number in '${CMAKE_CURRENT_SOURCE_DIR}/package.json'.")
endif(NOT EXPRESSCPP_PROJECT_VERSION)

string(
  REGEX
  REPLACE ".*\"(.*)\".*"
          "\\1"
          EXPRESSCPP_PROJECT_VERSION
          "${EXPRESSCPP_PROJECT_VERSION}")
set(PROJECT_VERSION ${EXPRESSCPP_PROJECT_VERSION})

message(STATUS "PROJECT_VERSION: ${PROJECT_VERSION}")

###
### options
###

option(EXPRESSCPP_USE_CONAN_DEPENDENCIES
       "Get dependencies from conan"
       OFF)
option(EXPRESSCPP_BUILD_EXAMPLES
       "Build the example executables"
       OFF)
option(EXPRESSCPP_BUILD_TESTS
       "Build the unit executables"
       OFF)
option(EXPRESSCPP_ENABLE_COVERAGE
       "Build with coverage support"
       OFF)
option(EXPRESSCPP_USE_STACKTRACE
       "Build with stacktrace support"
       OFF)
option(EXPRESSCPP_RUN_CLANG_TIDY
       "Use Clang-Tidy for static analysis"
       OFF)
option(EXPRESSCPP_USE_ADDRESS_SANITIZER
       "use address senitizer"
       OFF)
option(EXPRESSCPP_USE_MEMORY_SANITIZER
       "use memory SANITIZER"
       OFF)
option(EXPRESSCPP_USE_LEAK_SANITIZER
       "use leak SANITIZER"
       OFF)
option(EXPRESSCPP_USE_UNDEFINED_SANITIZER
       "use undefined SANITIZER"
       OFF)
option(EXPRESSCPP_USE_THREAD_SANITIZER
       "use thread SANITIZER"
       OFF)

if(CMAKE_BUILD_TYPE STREQUAL "")
  message(STATUS "CMAKE_BUILD_TYPE empty setting to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(MSVC)
  message(STATUS "using msvc")
  add_definitions(-D_WIN32_WINNT=0x600)
  add_definitions(-DBOOST_UUID_RANDOM_PROVIDER_FORCE_WINCRYPT)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(NOT BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS OFF)
endif()

if(EXPRESSCPP_ENABLE_COVERAGE)
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(WARNING "Code coverage results with an optimised (non-Debug) build may be misleading")
  endif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # using Clang
    message(STATUS "Not doing coverage...")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    message(STATUS "Building with code coverage...")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g -O0 --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -g -O0 --coverage -fprofile-arcs -ftest-coverage ")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage ")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage ")
    link_libraries(-lgcov)
  endif()
endif()

# Append our module directory to CMake
list(APPEND CMAKE_MODULE_PATH
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH
            ${CMAKE_BINARY_DIR})

if(NOT WIN32)
  include(sanitizers)
endif()

# Set the output of the libraries and executables.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

###
### package manager
###

if(EXPRESSCPP_USE_CONAN_DEPENDENCIES)
  message(STATUS "using conan dependencies")
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/conan.cmake)
  conan_cmake_run(CONANFILE
                  conanfile.txt
                  BASIC_SETUP
                  BUILD
                  missing)
  conan_basic_setup(TARGETS)
else()
  message(STATUS "EXPRESSCPP: not calling conan from cmaka")
endif()

###
### static analysis
###

if(EXPRESSCPP_RUN_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES "clang-tidy" DOC "/usr/bin/clang-tidy")
  if(NOT CLANG_TIDY_EXE)
    message(WARNING "clang-tidy not found.")
  else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}")
  endif()
endif(EXPRESSCPP_RUN_CLANG_TIDY)

###
### dependencies
###

find_package(Threads
             REQUIRED)

# TODO(gocarlos): we should also depend on boost beast here, conan not ready yet
find_package(Boost
             REQUIRED
             COMPONENTS system)
find_package(fmt)

find_package(nlohmann_json
            REQUIRED)


###
### library
###

add_library(${PROJECT_NAME}
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/expresscpp.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/http_method.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/impl/listener.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/impl/session.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/impl/routing_stack.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/impl/utils.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/nextrouter.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/request.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/response.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/route.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/types.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/exports.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/date.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/url.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/options.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/fetch.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/console.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/path_to_regexp.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/handlerfunctor.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/key.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/layer.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/path_to_regexp.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/expresscpp.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/url.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/fetch.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/date.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/console.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/handlerfunctor.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/route.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/http_method.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/nextrouter.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/listener.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/request.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/response.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/router.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/session.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/middleware/serve_static_provider.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/middleware/serve_favicon_provider.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/middleware/serve_favicon_provider.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/middleware/serve_static_provider.cpp)
target_include_directories(${PROJECT_NAME}
                           PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
                                  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                           PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_link_libraries(${PROJECT_NAME}
                      PUBLIC nlohmann_json::nlohmann_json
                             $<TARGET_NAME_IF_EXISTS:Boost::system>
                      PRIVATE fmt::fmt)
target_compile_options(
  ${PROJECT_NAME}
  PRIVATE $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall>
          $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-pedantic>
          $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra>
        )
target_compile_definitions(
          ${PROJECT_NAME}
          PUBLIC
          $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:EXPRESSCPP_STATIC_DEFINE>
          EXPRESSCPP_EXPORTS=1
        )
if(BUILD_SHARED_LIBS) 
  set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
      C_VISIBILITY_PRESET hidden)
endif()
if(CLANG_TIDY_EXE)
  set_target_properties(${PROJECT_NAME}
                        PROPERTIES CXX_CLANG_TIDY
                                   "${DO_CLANG_TIDY}")
endif()

if(EXPRESSCPP_USE_STACKTRACE)
  message(STATUS "using boost stack trace")
  add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)
  add_definitions(-DEXPRESSCPP_USE_STACKTRACE)
  target_compile_options(
    ${PROJECT_NAME}
    PUBLIC -fno-pie
           $<$<CXX_COMPILER_ID:GNU>:-no-pie>
           $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fPIC>)
  target_link_libraries(${PROJECT_NAME}
                        PUBLIC # for boost stackstrace
                               ${CMAKE_DL_LIBS})
endif()
add_library(${PROJECT_NAME}::${PROJECT_NAME}
            ALIAS
            ${PROJECT_NAME})

###
### examples
###

if(EXPRESSCPP_BUILD_EXAMPLES)
  message(STATUS "building examples")
  add_subdirectory(example)
endif()

###
### tests
###

if(EXPRESSCPP_BUILD_TESTS)
  message(STATUS "building tests")
  enable_testing()
  find_package(GTest)
  add_subdirectory(test)
endif()

###
### install
###

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(EXPRESSCPP_CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT lib)

install(EXPORT ${PROJECT_NAME}Targets
        DESTINATION ${EXPRESSCPP_CONFIG_INSTALL_DIR}
        NAMESPACE ${PROJECT_NAME}::
        COMPONENT dev)

configure_package_config_file(cmake/Config.cmake.in
                              ${PROJECT_NAME}Config.cmake
                              INSTALL_DESTINATION
                              ${EXPRESSCPP_CONFIG_INSTALL_DIR})
write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${EXPRESSCPP_CONFIG_INSTALL_DIR}
        COMPONENT dev)
install(DIRECTORY include/
                  ${CMAKE_CURRENT_BINARY_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT dev
        FILES_MATCHING
        PATTERN "*.hpp")

message(STATUS "-------------------------------------------------------")
message(STATUS "PROJECT_VERSION:.....................${PROJECT_VERSION}")
message(STATUS "EXPRESSCPP_USE_CONAN_DEPENDENCIES:...${EXPRESSCPP_USE_CONAN_DEPENDENCIES}")
message(STATUS "EXPRESSCPP_BUILD_TESTS:..............${EXPRESSCPP_BUILD_TESTS}")
message(STATUS "EXPRESSCPP_BUILD_EXAMPLES:...........${EXPRESSCPP_BUILD_EXAMPLES}")
message(STATUS "CMAKE_VERSION:.......................${CMAKE_VERSION}")
message(STATUS "CMAKE_C_COMPILER:....................${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER:..................${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_BUILD_TYPE:....................${CMAKE_BUILD_TYPE}")
message(STATUS "BUILD_SHARED_LIBS:...................${BUILD_SHARED_LIBS}")
message(STATUS "CLANG_TIDY_EXE:......................${CLANG_TIDY_EXE}")
message(STATUS "-------------------------------------------------------")