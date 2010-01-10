set (PROJECTNAME Wrapper)

include_directories(
	.
)
# define the sources
include(${PROJECTNAME}.list)

add_definitions("-DMYGUI_SOURCE_DIR=\"${MYGUI_SOURCE_DIR}\"")

# setup MyGUIEngine target
add_executable(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(${PROJECTNAME} PROPERTIES SOLUTION_FOLDER "Wrapper")

# add dependencies
#add_dependencies(${PROJECTNAME} MyGUIEngine )

mygui_config_sample(${PROJECTNAME})

# install debug pdb files
install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_DEBUG_PATH}/${PROJECTNAME}.pdb
	DESTINATION bin${MYGUI_DEBUG_PATH} CONFIGURATIONS Debug
)
install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_RELWDBG_PATH}/${PROJECTNAME}.pdb
	DESTINATION bin${MYGUI_RELWDBG_PATH} CONFIGURATIONS RelWithDebInfo
)

#mygui_install_target(${PROJECTNAME} "")
