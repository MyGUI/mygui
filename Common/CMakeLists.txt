set (PROJECT_NAME MyGUIFramework)

include_directories(
	.
	${MYGUI_SOURCE_DIR}/Common
	${MYGUI_SOURCE_DIR}/Common/Base
	${MYGUI_SOURCE_DIR}/MyGUIEngine/include
	${OGRE_INCLUDE_DIR}
)

if(MYGUI_RENDERSYSTEM EQUAL 1)
	add_definitions("-DMYGUI_DIRECTX_PLATFORM")
	include_directories(
		${MYGUI_SOURCE_DIR}/Platforms/DirectX/DirectXPlatform/include
	)
elseif(MYGUI_RENDERSYSTEM EQUAL 2)
	add_definitions("-DMYGUI_OGRE_PLATFORM")
	include_directories(
		${MYGUI_SOURCE_DIR}/Platforms/Ogre/OgrePlatform/include
	)

	set (HEADER_FILES ${HEADER_FILES} RenderBox/RenderBoxWrap.h)
	set (SOURCE_FILES ${SOURCE_FILES} RenderBox/RenderBoxWrap.cpp)
	SOURCE_GROUP("Source Files" FILES
	  RenderBox/RenderBoxWrap.cpp
	)
	SOURCE_GROUP("Header Files" FILES
	  RenderBox/RenderBoxWrap.h
	)
elseif(MYGUI_RENDERSYSTEM EQUAL 3)
	add_definitions("-DMYGUI_OPENGL_PLATFORM")
	include_directories(
		${MYGUI_SOURCE_DIR}/Platforms/OpenGL/OpenGLPlatform/include
	)
endif()

include(${PROJECT_NAME}.list)

# setup MyGUIEngine target
add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(${PROJECT_NAME} MyGUIEngine)

if(MYGUI_RENDERSYSTEM EQUAL 1)
	add_dependencies(${PROJECT_NAME} MyGUI.DirectXPlatform)
	target_link_libraries(${PROJECT_NAME} MyGUI.DirectXPlatform)
elseif(MYGUI_RENDERSYSTEM EQUAL 2)
	add_dependencies(${PROJECT_NAME} MyGUI.OgrePlatform)
	target_link_libraries(${PROJECT_NAME} MyGUI.OgrePlatform)
elseif(MYGUI_RENDERSYSTEM EQUAL 3)
	add_dependencies(${PROJECT_NAME} MyGUI.OpenGLPlatform)
	target_link_libraries(${PROJECT_NAME} MyGUI.OpenGLPlatform)
endif()