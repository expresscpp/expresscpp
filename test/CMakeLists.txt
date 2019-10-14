include(CTest)
enable_testing()

add_executable(${PROJECT_NAME}_test
    basic.cpp
    test.cpp)

target_link_libraries(${PROJECT_NAME}_test
                      ${PROJECT_NAME}
                      gtest::gtest
                      Threads::Threads)

add_test(${PROJECT_NAME}_test
         ${PROJECT_NAME}_test)
