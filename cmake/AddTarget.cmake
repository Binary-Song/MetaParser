include(CMakeParseArguments)

# example:
# AddTarget(
#   exe
#   name      InputResolver
#   src       "input_resovler.cpp"
#   includes  "."
#   libs      Shared
# )

function(AddTarget)
    cmake_parse_arguments(
        "arg"  # prefix
        "exe"  # optional args
        "name"   # one value args
        "libs;includes;src;"  # multi value args
        ${ARGN}
    )

    if(arg_exe)
        add_executable("${arg_name}" ${arg_src})
    else()
        add_library("${arg_name}" ${arg_src})
    endif()

    target_include_directories("${arg_name}" PUBLIC ${arg_includes})
    target_link_libraries("${arg_name}" PUBLIC ${arg_libs})

endfunction(AddTarget) 