set (PROJECT_NAME MyGUIFramework)

include_directories(
	.
	${MYGUI_SOURCE_DIR}/Common
	${MYGUI_SOURCE_DIR}/Common/Base
	${MYGUI_SOURCE_DIR}/MyGUIEngine/include
	${OGRE_INCLUDE_DIR}
)

if(MYGUI_BUILD_RENDERSYSTEM_OGRE)
	add_definitions("-DMYGUI_OGRE_PLATFORM")
	include_directories(
		${MYGUI_SOURCE_DIR}/Platforms/Ogre/OgrePlatform/include
	)
endif(MYGUI_BUILD_RENDERSYSTEM_OGRE)

include(${PROJECT_NAME}.list)

add_definitions("-DMYGUI_OGRE_PLATFORM")

# setup MyGUIEngine target
add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(${PROJECT_NAME} MyGUIEngine)

if(MYGUI_BUILD_RENDERSYSTEM_OGRE)
	add_dependencies(${PROJECT_NAME} MyGUI.OgrePlatform)
	target_link_libraries(${PROJECT_NAME} MyGUI.OgrePlatform)
endif(MYGUI_BUILD_RENDERSYSTEM_OGRE)