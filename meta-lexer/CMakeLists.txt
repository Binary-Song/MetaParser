add_library(meta-lexer 
    meta-lexer.cpp)
target_include_directories(meta-lexer PUBLIC
    ".")
target_link_libraries(meta-lexer PUBLIC 
    shared)

if(BUILD_TESTING)

    add_executable(meta-lexer-test 
        meta-lexer-test.cpp)
    target_link_libraries(meta-lexer-test PUBLIC 
        meta-lexer 
        input-resolver)

endif()