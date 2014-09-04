set (PROJECTNAME MyGUI.DummyPlatform)

include_directories(
	include
	${MYGUI_SOURCE_DIR}/MyGUIEngine/include
	${MYGUI_SOURCE_DIR}/Common
)

include(${PROJECTNAME}.list)

add_library(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(${PROJECTNAME} MyGUIEngine)

target_link_libraries(${PROJECTNAME} MyGUIEngine)

# installation rules
install(FILES ${HEADER_FILES}
        DESTINATION include/MYGUI
)
mygui_install_target(${PROJECTNAME} "")
