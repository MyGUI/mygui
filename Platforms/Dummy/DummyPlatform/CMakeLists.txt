set (PROJECTNAME MyGUI.DummyPlatform)

include_directories(
	include
	${MYGUI_SOURCE_DIR}/MyGUIEngine/include
	${MYGUI_SOURCE_DIR}/Common
)

include(${PROJECTNAME}.list)

add_library(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(${PROJECTNAME} MyGUIEngine)
