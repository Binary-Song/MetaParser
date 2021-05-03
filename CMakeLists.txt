cmake_minimum_required(VERSION 3.1)
project(meta-parser)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(RunDoxygen)


# configs

set(GENERATE_DOCS NO CACHE BOOL "是否生成文档")  
configure_file("${CMAKE_CURRENT_LIST_DIR}/docs/Doxyfile.in" "${CMAKE_CURRENT_LIST_DIR}/docs/Doxyfile")
configure_file("${CMAKE_CURRENT_LIST_DIR}/shared/local_path.hpp.in" "${CMAKE_CURRENT_LIST_DIR}/shared/local_path.hpp")

# compiler options

set(CMAKE_CXX_STANDARD_REQUIRED ON ) # standard mandatory
set(CMAKE_CXX_EXTENSIONS        OFF ) # no gnu c++
set(CMAKE_CXX_STANDARD          14) # min standard 

if(MSVC) 
    add_compile_options(/utf-8 )
endif()


# module settings
add_subdirectory(third_party)
add_subdirectory(shared)
add_subdirectory(meta-lexer)
add_subdirectory(meta-parser)
add_subdirectory(input-resolver)
add_subdirectory(examples)

# docs

if(GENERATE_DOCS)
    RunDoxygen()
endif()