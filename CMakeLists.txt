cmake_minimum_required(VERSION 3.8)

project(expresscpp CXX)

option(BUILD_TESTS
       "Build the unit executables"
       ON)

option (BUILD_EXAMPLE "Build the example executables" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")

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
find_package(gtest
             REQUIRED)
find_package(Threads
             REQUIRED)

add_library(${PROJECT_NAME}
            include/${PROJECT_NAME}/expresscpp.hpp
            include/${PROJECT_NAME}/nextrouter.hpp
            include/${PROJECT_NAME}/request.hpp
            include/${PROJECT_NAME}/response.hpp
            include/${PROJECT_NAME}/impl/listener.hpp
            include/${PROJECT_NAME}/impl/session.hpp
            include/${PROJECT_NAME}/impl/utils.hpp
            src/expresscpp.cpp
            src/utils.cpp
            src/session.cpp
            src/listener.cpp
            src/response.cpp
            src/request.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include/)

if(BUILD_EXAMPLE)
add_subdirectory(example)
endif()

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
