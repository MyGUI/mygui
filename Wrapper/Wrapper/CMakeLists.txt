set (PROJECTNAME Wrapper)

include_directories(
	.
)
# define the sources
include(${PROJECTNAME}.list)

add_definitions("-DMYGUI_SOURCE_DIR=\"${MYGUI_SOURCE_DIR}\"")

add_executable(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(${PROJECTNAME} PROPERTIES SOLUTION_FOLDER "Wrapper")

# add dependencies
#add_dependencies(${PROJECTNAME} MyGUIEngine )

mygui_config_sample(${PROJECTNAME})

#mygui_install_app(${PROJECTNAME})
