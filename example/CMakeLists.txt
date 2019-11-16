list(APPEND CMAKE_MODULE_PATH
            ${CMAKE_CURRENT_LIST_DIR})
include(embed_favicon_in_header_file)

create_resources(${CMAKE_CURRENT_LIST_DIR}
                 ${CMAKE_BINARY_DIR}/favicon_dump.hpp)

function(add_example
         Name)
  add_executable(example_${Name} ${CMAKE_CURRENT_LIST_DIR}/${Name}.cpp)
  target_link_libraries(example_${Name}
                        PRIVATE ${PROJECT_NAME}
                                ${CMAKE_THREAD_LIBS_INIT}
                                Threads::Threads
                                $<$<PLATFORM_ID:Linux>:-lm>
                                $<$<PLATFORM_ID:Linux>:-pthread>)
  target_compile_options(
    example_${Name}
    PRIVATE $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall>
            $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-pedantic>
            $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra>)
  if(CLANG_TIDY_EXE)
    set_target_properties(example_${Name}
                          PROPERTIES CXX_CLANG_TIDY
                                     "${DO_CLANG_TIDY}")
  endif()
endfunction()

add_example(middleware_auth_like)
add_example(middleware_logger_like)
add_example(query_params)
add_example(url_params)
add_example(simple_rest_router_server)
add_example(error_handler)
add_example(multiple_handlers)
add_example(router)
add_example(serve_favicon)
add_example(serve_static)
