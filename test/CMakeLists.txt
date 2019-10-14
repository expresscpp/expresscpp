include(CTest)
enable_testing()

add_executable(${PROJECT_NAME}_test
               basic.cpp
               hello_world.cpp
               helper.cpp
               helper.hpp
               test.cpp)

target_link_libraries(${PROJECT_NAME}_test
                      ${PROJECT_NAME}
                      gtest::gtest
                      Threads::Threads)

add_test(${PROJECT_NAME}_test
         ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}_test)
