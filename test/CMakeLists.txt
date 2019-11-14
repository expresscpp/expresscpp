include(CTest)

add_executable(test_${PROJECT_NAME}
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
               test_main.cpp
               $<$<BOOL:${EXPRESSCPP_ENABLE_STATIC_FILE_PROVIDER}>:static_file_middleware.cpp>)

target_link_libraries(test_${PROJECT_NAME}
                      ${PROJECT_NAME}
                      gtest::gtest
                      ${Boost_LIBRARIES}
                      Threads::Threads)

add_test(test_${PROJECT_NAME}
         ${CMAKE_BINARY_DIR}/bin/test_${PROJECT_NAME})
