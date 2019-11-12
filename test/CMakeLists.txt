include(CTest)

add_executable(test_${PROJECT_NAME} "")
target_sources(test_${PROJECT_NAME}
               PRIVATE
               layer.cpp
               basic.cpp
               routing.cpp
               hello_world.cpp
               console.cpp
               long_session.cpp
               error_handler.cpp
               path_to_regexp.cpp
               date.cpp
               http_methods.cpp
               middleware.cpp
               routing_table.cpp
               test_main.cpp)

if(EXPRESSCPP_ENABLE_STATIC_FILE_PROVIDER)
  target_sources(test_${PROJECT_NAME}
                 PRIVATE
                 static_file_middleware.cpp)
endif()

target_link_libraries(test_${PROJECT_NAME}
                      ${PROJECT_NAME}
                      gtest::gtest
                      ${Boost_LIBRARIES}
                      Threads::Threads)

# message(FATAL_ERROR "CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}")

add_test(test_${PROJECT_NAME}
         ${CMAKE_BINARY_DIR}/bin/test_${PROJECT_NAME})
