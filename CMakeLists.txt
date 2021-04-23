cmake_minimum_required(VERSION  3.1)
project(meta-parser)
if(MSVC)
    message("Using msvc")
    add_compile_options(/utf-8)
endif()


add_subdirectory(io)