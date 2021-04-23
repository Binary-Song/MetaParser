add_executable(IO io.cpp)
target_include_directories(IO PUBLIC ".")
target_link_libraries(IO PUBLIC shared)