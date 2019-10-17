include(CTest)
enable_testing()

add_executable(${PROJECT_NAME}_test
               basic.cpp
               hello_world.cpp
               http_methods.cpp
               routing_table.cpp
               static_file_middleware.cpp
               test_main.cpp
               test_utils.cpp
               test_utils.hpp)

target_link_libraries(${PROJECT_NAME}_test
                      ${PROJECT_NAME}
                      gtest::gtest
                      Threads::Threads)

add_test(${PROJECT_NAME}_test
         ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}_test)
