function(mygui_add_base_manager_source PLATFORM)
	include_directories(Base/${PLATFORM})
	set (HEADER_FILES ${HEADER_FILES} Base/${PLATFORM}/BaseManager.h PARENT_SCOPE)
	set (SOURCE_FILES ${SOURCE_FILES} Base/${PLATFORM}/BaseManager.cpp PARENT_SCOPE)
	SOURCE_GROUP("Base" FILES
		Base/${PLATFORM}/BaseManager.h
		Base/${PLATFORM}/BaseManager.cpp
	)
endfunction(mygui_add_base_manager_source)

function(mygui_add_input_source PLATFORM)
	include_directories(Input/${PLATFORM})
	set (HEADER_FILES ${HEADER_FILES}
		Input/${PLATFORM}/InputManager.h
		Input/${PLATFORM}/PointerManager.h
		PARENT_SCOPE)
	set (SOURCE_FILES ${SOURCE_FILES}
		Input/${PLATFORM}/InputManager.cpp
		Input/${PLATFORM}/PointerManager.cpp
		PARENT_SCOPE)
	SOURCE_GROUP("Base" FILES
		Input/${PLATFORM}/InputManager.h
		Input/${PLATFORM}/InputManager.cpp
		Input/${PLATFORM}/PointerManager.h
		Input/${PLATFORM}/PointerManager.cpp
	)
endfunction(mygui_add_input_source)

set (PROJECTNAME Common)

include_directories(
	.
	${MYGUI_SOURCE_DIR}/Common
	${MYGUI_SOURCE_DIR}/MyGUIEngine/include
)

set (HEADER_FILES "")
set (SOURCE_FILES "")

if(MYGUI_RENDERSYSTEM EQUAL 1)
	mygui_add_base_manager_source(Dummy)
	add_definitions("-DMYGUI_DUMMY_PLATFORM")
	include_directories(
		${MYGUI_SOURCE_DIR}/Platforms/Dummy/DummyPlatform/include
	)
	link_directories(${DUMMY_LIB_DIR})
elseif(MYGUI_RENDERSYSTEM EQUAL 3)
	mygui_add_base_manager_source(Ogre)
	add_definitions("-DMYGUI_OGRE_PLATFORM")
	include_directories(
		${MYGUI_SOURCE_DIR}/Platforms/Ogre/OgrePlatform/include
		${OGRE_INCLUDE_DIR}
	)
	link_directories(${OGRE_LIB_DIR})
elseif(MYGUI_RENDERSYSTEM EQUAL 4)
	mygui_add_base_manager_source(OpenGL)
	add_definitions("-DMYGUI_OPENGL_PLATFORM")
	include_directories(
		${MYGUI_SOURCE_DIR}/Platforms/OpenGL/OpenGLPlatform/include
		${OPENGL_INCLUDE_DIR}
	)
	link_directories(${OPENGL_LIB_DIR})
elseif(MYGUI_RENDERSYSTEM EQUAL 5)
	mygui_add_base_manager_source(DirectX)
	add_definitions("-DMYGUI_DIRECTX_PLATFORM")
	include_directories(
		${MYGUI_SOURCE_DIR}/Platforms/DirectX/DirectXPlatform/include
		${DirectX_INCLUDE_DIR}
	)
	link_directories(${DIRECTX_LIB_DIR})
elseif(MYGUI_RENDERSYSTEM EQUAL 6)
	mygui_add_base_manager_source(DirectX11)
	add_definitions("-DMYGUI_DIRECTX11_PLATFORM")
	include_directories(
		${MYGUI_SOURCE_DIR}/Platforms/DirectX11/DirectX11Platform/include
		${DirectX_INCLUDE_DIR}
	)
	link_directories(${DIRECTX_LIB_DIR})
endif()

if(MYGUI_SAMPLES_INPUT EQUAL 1)
	include_directories(${OIS_INCLUDE_DIRS})
	mygui_add_input_source(OIS)
elseif(MYGUI_SAMPLES_INPUT EQUAL 2)
	mygui_add_input_source(Win32)
elseif(MYGUI_SAMPLES_INPUT EQUAL 3)
	include_directories(${OIS_INCLUDE_DIRS})
	mygui_add_input_source(Win32_OIS)
endif()

add_library(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})

if(MYGUI_RENDERSYSTEM EQUAL 1)
	add_dependencies(${PROJECTNAME} MyGUI.DummyPlatform)
	target_link_libraries(${PROJECTNAME} MyGUI.DummyPlatform)
elseif(MYGUI_RENDERSYSTEM EQUAL 3)
	add_dependencies(${PROJECTNAME} MyGUI.OgrePlatform)
	target_link_libraries(${PROJECTNAME} MyGUI.OgrePlatform)
elseif(MYGUI_RENDERSYSTEM EQUAL 4)
	add_dependencies(${PROJECTNAME} MyGUI.OpenGLPlatform)
	target_link_libraries(${PROJECTNAME} MyGUI.OpenGLPlatform)
elseif(MYGUI_RENDERSYSTEM EQUAL 5)
	add_dependencies(${PROJECTNAME} MyGUI.DirectXPlatform)
	target_link_libraries(${PROJECTNAME} MyGUI.DirectXPlatform)
elseif(MYGUI_RENDERSYSTEM EQUAL 6)
	add_dependencies(${PROJECTNAME} MyGUI.DirectX11Platform)
	target_link_libraries(${PROJECTNAME} MyGUI.DirectX11Platform)
endif()

add_dependencies(${PROJECTNAME} MyGUIEngine)

if(MYGUI_SAMPLES_INPUT EQUAL 1)
	add_definitions("-DMYGUI_SAMPLES_INPUT_OIS")
	link_directories(${OIS_LIB_DIR})
	target_link_libraries(${PROJECTNAME} ${OIS_LIBRARIES})
elseif(MYGUI_SAMPLES_INPUT EQUAL 2)
	add_definitions("-DMYGUI_SAMPLES_INPUT_WIN32")
elseif(MYGUI_SAMPLES_INPUT EQUAL 3)
	add_definitions("-DMYGUI_SAMPLES_INPUT_WIN32_OIS")
	link_directories(${OIS_LIB_DIR})
	target_link_libraries(${PROJECTNAME} ${OIS_LIBRARIES})
endif()

if(NOT WIN32)
	find_package(X11)
	target_link_libraries(${PROJECTNAME} X11)
	include_directories(${X11_INCLUDE_DIR})
endif()
