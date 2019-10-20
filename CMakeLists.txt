cmake_minimum_required(VERSION 3.8)

project(expresscpp LANGUAGES CXX)

option(EXPRESSCPP_BUILD_EXAMPLE
       "Build the example executables"
       OFF)
option(EXPRESSCPP_BUILD_TESTS
       "Build the unit executables"
       OFF)
option(EXPRESSCPP_ENABLE_COVERAGE
       "Build with coverage support"
       OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")

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

# Set the output of the libraries and executables.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include(${CMAKE_SOURCE_DIR}/cmake/conan.cmake)
conan_cmake_run(CONANFILE
                conanfile.txt
                BASIC_SETUP
                BUILD
                missing)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# TODO: we should also depend on boost beast here, conan not ready yet
find_package(Boost
             REQUIRED
             COMPONENTS system)
find_package(Threads
             REQUIRED)
find_package(fmt)
find_package(nlohmann_json
             REQUIRED)

add_library(${PROJECT_NAME}
            include/${PROJECT_NAME}/expresscpp.hpp
            include/${PROJECT_NAME}/http_method.hpp
            include/${PROJECT_NAME}/impl/listener.hpp
            include/${PROJECT_NAME}/impl/session.hpp
            include/${PROJECT_NAME}/impl/utils.hpp
            include/${PROJECT_NAME}/middleware/staticfileprovider.hpp
            include/${PROJECT_NAME}/nextrouter.hpp
            include/${PROJECT_NAME}/request.hpp
            include/${PROJECT_NAME}/response.hpp
            include/${PROJECT_NAME}/types.hpp
            src/expresscpp.cpp
            src/http_method.cpp
            src/listener.cpp
            src/middleware/staticfileprovider.cpp
            src/request.cpp
            src/response.cpp
            src/router.cpp
            src/session.cpp
            src/utils.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include/)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json)

if(EXPRESSCPP_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

if(EXPRESSCPP_BUILD_TESTS)
  find_package(gtest
               REQUIRED)
  add_subdirectory(test)
endif()
