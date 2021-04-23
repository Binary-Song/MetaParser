add_library(meta-parser meta-parser.cpp)
target_include_directories(meta-parser PUBLIC ".")
target_link_libraries(meta-parser PUBLIC shared)
add_executable(meta-parser-test meta-parser-test.cpp)
