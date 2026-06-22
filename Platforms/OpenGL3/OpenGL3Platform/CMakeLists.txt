set(PROJECTNAME MyGUI.OpenGL3Platform)

include_directories(
	include
	"${MYGUI_SOURCE_DIR}/Common"
)

include(${PROJECTNAME}.list)

add_definitions(-DGL_GLEXT_PROTOTYPES)

add_library(${PROJECTNAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECTNAME} OpenGL::GL MyGUIEngine)

# installation rules
install(FILES ${HEADER_FILES}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
)
mygui_install_target(${PROJECTNAME})
