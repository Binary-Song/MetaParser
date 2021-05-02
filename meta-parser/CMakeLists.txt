add_library(meta-parser 
    "./empderv/empderv.cpp"
    "./first/first.cpp"
    "./follow/follow.cpp"
    "./select/select.cpp"
    "./prdtbl/prdtbl.cpp"
    )

target_include_directories(meta-parser PUBLIC
    "."
    "prdtbl")

target_link_libraries(meta-parser PUBLIC 
    meta-lexer
    shared)

add_library(meta-parser-test-interface INTERFACE)

target_link_libraries(meta-parser-test-interface INTERFACE 
    meta-parser
    input-resolver)

add_executable(meta-parser-empderv-test
    "./empderv/empderv-test.cpp")

add_executable(meta-parser-first-test
    "./first/first-test.cpp")
 
add_executable(meta-parser-follow-test
    "./follow/follow-test.cpp"
)

add_executable(meta-parser-select-test
    "./select/select-test.cpp"
)

target_link_libraries(meta-parser-empderv-test PUBLIC 
    meta-parser-test-interface
)

target_link_libraries(meta-parser-first-test PUBLIC 
    meta-parser-test-interface
)

target_link_libraries(meta-parser-follow-test PUBLIC 
    meta-parser-test-interface
)

target_link_libraries(meta-parser-select-test PUBLIC 
    meta-parser-test-interface
)