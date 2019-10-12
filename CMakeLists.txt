cmake_minimum_required(VERSION 3.8)

project(expresscpp)

option(BUILD_TESTS
       "ASDF"
       ON)


option (BUILD_EXAMPLE "asdf" ON)

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

add_library(${PROJECT_NAME}
            include/${PROJECT_NAME}/expresscpp.hpp
            include/${PROJECT_NAME}/nextrouter.hpp
            include/${PROJECT_NAME}/request.hpp
            include/${PROJECT_NAME}/response.hpp
            src/expresscpp.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include/)

find_package(gtest
             REQUIRED)
find_package(Threads
             REQUIRED)

if(BUILD_EXAMPLE)
add_executable(${PROJECT_NAME}_example example/main.cpp)

target_link_libraries(${PROJECT_NAME}_example
                     ${PROJECT_NAME}
                     Threads::Threads)
endif()

if(BUILD_TESTS)
  include(CTest)
  enable_testing()

  add_executable(${PROJECT_NAME}_test test/test.cpp)

  target_link_libraries(${PROJECT_NAME}_test
                        ${PROJECT_NAME}
                        gtest::gtest
                        Threads::Threads)

  add_test(${PROJECT_NAME}_test
           ${PROJECT_NAME}_test)

endif()
