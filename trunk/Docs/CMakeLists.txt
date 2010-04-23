
INCLUDE( ${CMAKE_ROOT}/Modules/FindDoxygen.cmake)
CONFIGURE_FILE(${MYGUI_SOURCE_DIR}/Docs/Doxyfile.in ${MYGUI_BINARY_DIR}/Docs/Doxyfile @ONLY IMMEDIATE)
add_custom_target (api-docs doxygen ${MYGUI_BINARY_DIR}/Docs/Doxyfile WORKING_DIRECTORY ${MYGUI_BINARY_DIR}/Docs/ COMMENT "Generating documentation")
