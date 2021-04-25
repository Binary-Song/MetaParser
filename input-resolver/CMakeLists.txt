add_library(input-resolver 
    "input-resolver.cpp")
    
target_include_directories(input-resolver 
PUBLIC
    ".")

target_link_libraries(input-resolver 
PUBLIC 
    shared)

add_executable(input-resolver-test
    "input-resolver-test.cpp")

target_link_libraries(input-resolver-test 
PUBLIC 
    input-resolver)