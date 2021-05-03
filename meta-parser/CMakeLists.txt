# meta-parser目标和相关配置

add_library(meta-parser 
    "./empderv/empderv.cpp"
    "./first/first.cpp"
    "./follow/follow.cpp"
    "./select/select.cpp"
    "./prdtbl/prdtbl.cpp"
    "./meta-parser.cpp"
    )

target_include_directories(meta-parser PUBLIC
    "."
    "prdtbl")

target_link_libraries(meta-parser PUBLIC 
    meta-lexer
    shared)

# meta-parser-test-interface目标
# 所有meta-parser相关的测试目标都应该链接到此目标以便统一配置

add_library(meta-parser-test-interface INTERFACE)

target_link_libraries(meta-parser-test-interface INTERFACE 
    meta-parser
    input-resolver)


# meta-parser相关的测试目标 

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

add_executable(meta-parser-prdtbl-test
    "./prdtbl/prdtbl-test.cpp"
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

target_link_libraries(meta-parser-prdtbl-test PUBLIC 
    meta-parser-test-interface
)