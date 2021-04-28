cmake_minimum_required(VERSION 3.1)
project(meta-parser)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(RunDoxygen)
set(GENERATE_DOCS NO CACHE BOOL "是否生成文档")  



if(MSVC) 
    add_compile_options(/utf-8)
endif()
 

add_subdirectory(shared)
add_subdirectory(meta-lexer)
add_subdirectory(meta-parser)
add_subdirectory(input-resolver)
add_subdirectory(examples)

if(GENERATE_DOCS)
    RunDoxygen()
endif()