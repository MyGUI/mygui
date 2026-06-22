
include("${CMAKE_ROOT}/Modules/FindDoxygen.cmake")
configure_file("${MYGUI_SOURCE_DIR}/Docs/Doxyfile.in" "${MYGUI_BINARY_DIR}/Docs/Doxyfile" @ONLY)
add_custom_target(api-docs doxygen "${MYGUI_BINARY_DIR}/Docs/Doxyfile" WORKING_DIRECTORY "${MYGUI_BINARY_DIR}/Docs/" COMMENT "Generating documentation")
