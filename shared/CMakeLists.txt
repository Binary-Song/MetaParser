add_library(shared "syntax_tree.cpp")
target_include_directories(shared PUBLIC ".")
target_link_libraries(shared TextTable)