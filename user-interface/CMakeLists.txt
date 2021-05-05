include(AddTarget)

AddTarget(  
    exe 
    name     "user-interface"
    src      "main.cpp"
    libs     "meta-parser" "shared" "input-resolver"
    includes "."
)

set_target_properties("user-interface" PROPERTIES OUTPUT_NAME "parse")

