# Creates C resources file from files in given directory
function(create_resources dir output)
    # Create empty output file
    file(WRITE ${output} "")
    # Collect input files
    file(GLOB bins ${dir}/favicon.ico)
    # Iterate through input files
    foreach(bin ${bins})
        # Get short filename
        string(REGEX MATCH "([^/]+)$" filename ${bin})
        # Replace filename spaces & extension separator for C compatibility
        string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
        # Read hex data from file
        file(READ ${bin} filedata HEX)
        # Convert hex data for C compatibility
        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
        # Append data to output file
        file(APPEND ${output} "static const unsigned char ${filename}[] = {${filedata}};\nstatic const unsigned ${filename}_size = sizeof(${filename});\n")
    endforeach()
endfunction()

create_resources(${CMAKE_CURRENT_LIST_DIR} ${CMAKE_BINARY_DIR}/asdf.hpp)

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

if(EXPRESSCPP_ENABLE_STATIC_FILE_PROVIDER)
  add_example(static_file_server)
endif()
