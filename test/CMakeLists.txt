include(CTest)
enable_testing()

add_executable(test_${PROJECT_NAME} "")
target_sources(test_${PROJECT_NAME}
               PRIVATE
               basic.cpp
               routing.cpp
               hello_world.cpp
               console.cpp
               utils.cpp
               date.cpp
               http_methods.cpp
               middleware.cpp
               routing_table.cpp
               test_main.cpp
               test_utils.cpp
               test_utils.hpp)

if(EXPRESSCPP_ENABLE_STATIC_FILE_PROVIDER)
  target_sources(test_${PROJECT_NAME}
                 PRIVATE
                 static_file_middleware.cpp)
endif()

target_link_libraries(test_${PROJECT_NAME}
                      ${PROJECT_NAME}
                      gtest::gtest
                      Threads::Threads)

add_test(test_${PROJECT_NAME}
         ${CMAKE_BINARY_DIR}/bin/test_${PROJECT_NAME})
