add_executable(${PROJECT_NAME}_simple_rest_router_server simple_rest_router_server.cpp)
target_link_libraries(${PROJECT_NAME}_simple_rest_router_server
                      ${PROJECT_NAME}
                      Threads::Threads)
if(CLANG_TIDY_EXE)
  set_target_properties(${PROJECT_NAME}_simple_rest_router_server
                        PROPERTIES CXX_CLANG_TIDY
                                   "${DO_CLANG_TIDY}")
endif()

if(EXPRESSCPP_ENABLE_STATIC_FILE_PROVIDER)
  add_executable(${PROJECT_NAME}_simple_static_file_server simple_static_file_server.cpp)
  target_link_libraries(${PROJECT_NAME}_simple_static_file_server
                        ${PROJECT_NAME}
                        Threads::Threads)
  if(CLANG_TIDY_EXE)
    set_target_properties(${PROJECT_NAME}_simple_static_file_server
                          PROPERTIES CXX_CLANG_TIDY
                                     "${DO_CLANG_TIDY}")
  endif()
endif()
