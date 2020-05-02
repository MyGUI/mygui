function(mygui_add_base_manager_include PLATFORM_ID)
	mygui_set_platform_name(${PLATFORM_ID})
	include_directories(../../Common/Base/${MYGUI_PLATFORM_NAME})
endfunction(mygui_add_base_manager_include)

set (PROJECTNAME Common)

include_directories(
	.
	${MYGUI_SOURCE_DIR}/Common
	${MYGUI_SOURCE_DIR}/MyGUIEngine/include
)

set (HEADER_FILES "")
set (SOURCE_FILES "")

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS}/..)
link_directories(${SDL2_LIB_DIR})

mygui_set_platform_name(${MYGUI_RENDERSYSTEM})
set (HEADER_FILES ${HEADER_FILES} Base/${MYGUI_PLATFORM_NAME}/BaseManager.h Base/PlatformBaseManager/SdlBaseManager.h)
set (SOURCE_FILES ${SOURCE_FILES} Base/${MYGUI_PLATFORM_NAME}/BaseManager.cpp Base/PlatformBaseManager/SdlBaseManager.cpp)
SOURCE_GROUP("Base" FILES
	Base/${MYGUI_PLATFORM_NAME}/BaseManager.h
	Base/${MYGUI_PLATFORM_NAME}/BaseManager.cpp
	Base/PlatformBaseManager/SdlBaseManager.h
	Base/PlatformBaseManager/SdlBaseManager.cpp
)
include_directories(
	${MYGUI_SOURCE_DIR}/Platforms/${MYGUI_PLATFORM_NAME}/${MYGUI_PLATFORM_NAME}Platform/include
)
if(MYGUI_RENDERSYSTEM EQUAL 1)
	add_definitions("-DMYGUI_DUMMY_PLATFORM")
	link_directories(${DUMMY_LIB_DIR})
elseif(MYGUI_RENDERSYSTEM EQUAL 3)
	add_definitions("-DMYGUI_OGRE_PLATFORM")
	include_directories(
		${OGRE_INCLUDE_DIR}
	)
	link_directories(${OGRE_LIB_DIR})
elseif(MYGUI_RENDERSYSTEM EQUAL 4)
	add_definitions("-DMYGUI_OPENGL_PLATFORM")
	include_directories(
		${OPENGL_INCLUDE_DIR}
		${SDL2_IMAGE_INCLUDE_DIRS}
	)
	link_directories(${OPENGL_LIB_DIR})
	link_directories(${SDL2_IMAGE_LIB_DIR})
elseif(MYGUI_RENDERSYSTEM EQUAL 5)
	add_definitions("-DMYGUI_DIRECTX_PLATFORM")
	include_directories(
		${DirectX_INCLUDE_DIR}
	)
	link_directories(${DIRECTX_LIB_DIR})
elseif(MYGUI_RENDERSYSTEM EQUAL 6)
	add_definitions("-DMYGUI_DIRECTX11_PLATFORM")
	include_directories(
		${DirectX_INCLUDE_DIR}
	)
	link_directories(${DIRECTX_LIB_DIR})
elseif(MYGUI_RENDERSYSTEM EQUAL 7)
	add_definitions("-DMYGUI_OPENGL3_PLATFORM")
	include_directories(
		${OPENGL_INCLUDE_DIR}
		${SDL2_IMAGE_INCLUDE_DIRS}
	)
	link_directories(${OPENGL_LIB_DIR})
	link_directories(${SDL2_IMAGE_LIB_DIR})
elseif(MYGUI_RENDERSYSTEM EQUAL 8)
	add_definitions("-DMYGUI_OPENGLES_PLATFORM")
	include_directories(
			${OPENGL_INCLUDE_DIR}
			${SDL2_IMAGE_INCLUDE_DIRS}
	)
	link_directories(${OPENGL_LIB_DIR})
	link_directories(${SDL2_IMAGE_LIB_DIR})
endif()

include_directories(Input/SDL)
set (HEADER_FILES ${HEADER_FILES}
	Input/SDL/InputManager.h
	Input/SDL/PointerManager.h
	)
set (SOURCE_FILES ${SOURCE_FILES}
	Input/SDL/InputManager.cpp
	Input/SDL/PointerManager.cpp
	)
SOURCE_GROUP("Base" FILES
	Input/SDL/InputManager.h
	Input/SDL/InputManager.cpp
	Input/SDL/PointerManager.h
	Input/SDL/PointerManager.cpp
)

add_library(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})

mygui_set_platform_name(${MYGUI_RENDERSYSTEM})
add_dependencies(${PROJECTNAME} MyGUI.${MYGUI_PLATFORM_NAME}Platform)
target_link_libraries(${PROJECTNAME} MyGUI.${MYGUI_PLATFORM_NAME}Platform)

add_dependencies(${PROJECTNAME} MyGUIEngine)
