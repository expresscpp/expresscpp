add_executable(${PROJECT_NAME}_simple_rest_api_server simple_rest_api_server.cpp)
target_link_libraries(${PROJECT_NAME}_simple_rest_api_server
                      ${PROJECT_NAME}
                      Threads::Threads)

add_executable(${PROJECT_NAME}_simple_static_file_server simple_static_file_server.cpp)
target_link_libraries(${PROJECT_NAME}_simple_static_file_server
                      ${PROJECT_NAME}
                      Threads::Threads)
