include(AddTarget)

AddTarget(  
    name     "input-resolver"
    src      "input-resolver.cpp"
    libs     "shared"
    includes "."
)
if(BUILD_TESTING)
AddTarget(
    exe
    name    "input-resolver-test"
    src     "input-resolver-test.cpp"
    libs    "input-resolver"
)
endif()