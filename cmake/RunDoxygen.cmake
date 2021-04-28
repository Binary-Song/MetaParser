function(RunDoxygen)
    find_package(Doxygen REQUIRED)
 
    set(DOXYGEN_INDEX_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/docs/html/index.html")
    set(DOCS_FOLDER   "${CMAKE_CURRENT_SOURCE_DIR}/docs")
    set(DOXYFILE      "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile")

    add_custom_command(OUTPUT       ${DOXYGEN_INDEX_FILE}
    DEPENDS                         meta-parser
    COMMAND                         ${DOXYGEN_EXECUTABLE}  ${DOXYFILE}
    WORKING_DIRECTORY               ${DOCS_FOLDER}
    MAIN_DEPENDENCY                 ${DOXYFILE}
    COMMENT "Run doxygen.")

    add_custom_target(docs ALL DEPENDS ${DOXYGEN_INDEX_FILE})



    find_package(Sphinx REQUIRED)

    set(SPHINX_SOURCE ${DOCS_FOLDER})
    set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/docs/sphinx)

    add_custom_target(Sphinx ALL
                    COMMAND
                    ${SPHINX_EXECUTABLE} -b html
                    ${SPHINX_SOURCE} ${SPHINX_BUILD}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating documentation with Sphinx")
endfunction(RunDoxygen)
