set (PROJECTNAME MyGUICommon)

set (HEADER_FILES "")
set (SOURCE_FILES "")

mygui_set_platform_name(${MYGUI_RENDERSYSTEM})
set (HEADER_FILES ${HEADER_FILES} Base/${MYGUI_PLATFORM_NAME}/BaseManager.h Base/PlatformBaseManager/SdlBaseManager.h)
set (SOURCE_FILES ${SOURCE_FILES} Base/${MYGUI_PLATFORM_NAME}/BaseManager.cpp Base/PlatformBaseManager/SdlBaseManager.cpp)
SOURCE_GROUP("Base" FILES
	Base/${MYGUI_PLATFORM_NAME}/BaseManager.h
	Base/${MYGUI_PLATFORM_NAME}/BaseManager.cpp
	Base/PlatformBaseManager/SdlBaseManager.h
	Base/PlatformBaseManager/SdlBaseManager.cpp
)

set (HEADER_FILES ${HEADER_FILES}
	Input/SDL/InputManager.h
	Input/SDL/PointerManager.h
	Input/SDL/ResourceSDLPointer.h
)
set (SOURCE_FILES ${SOURCE_FILES}
	Input/SDL/InputManager.cpp
	Input/SDL/PointerManager.cpp
	Input/SDL/ResourceSDLPointer.cpp
)
SOURCE_GROUP("Base" FILES
	Input/SDL/InputManager.h
	Input/SDL/InputManager.cpp
	Input/SDL/PointerManager.h
	Input/SDL/PointerManager.cpp
	Input/SDL/ResourceSDLPointer.h
	Input/SDL/ResourceSDLPointer.cpp
)

add_library(${PROJECTNAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(${PROJECTNAME}
		# not useful for demos and tools
		PUBLIC
			-Wno-unique-object-duplication)
endif()

if(MYGUI_RENDERSYSTEM EQUAL 3)
	target_include_directories(${PROJECTNAME} SYSTEM PUBLIC "${OGRE_INCLUDE_DIRS}")
	target_link_directories(${PROJECTNAME} PUBLIC "${OGRE_LIBRARY_DIRS}")

	include(CheckIncludeFileCXX)
	set(CMAKE_REQUIRED_INCLUDES "${OGRE_INCLUDE_DIRS}")
	check_include_file_cxx("Bites/OgreStaticPluginLoader.h" MYGUI_HAVE_OGRE_BITES)
	if(OGRE_STATIC AND NOT MYGUI_HAVE_OGRE_BITES)
		message(FATAL_ERROR "Static Ogre requires OgreBites (Bites/OgreStaticPluginLoader.h not found)")
	endif()

elseif(MYGUI_RENDERSYSTEM EQUAL 5)
	target_link_libraries(${PROJECTNAME} PUBLIC DirectX::D3D9)
elseif(MYGUI_RENDERSYSTEM EQUAL 6)
	target_link_libraries(${PROJECTNAME} PUBLIC DirectX11::D3D11)
elseif(MYGUI_RENDERSYSTEM EQUAL 4 OR MYGUI_RENDERSYSTEM EQUAL 7 OR MYGUI_RENDERSYSTEM EQUAL 8)
	target_link_libraries(${PROJECTNAME} PRIVATE SDL2_image::SDL2_image)
endif()

target_include_directories(${PROJECTNAME}
	PUBLIC
		.
		Base/${MYGUI_PLATFORM_NAME}
)
string(TOUPPER "${MYGUI_PLATFORM_NAME}" MYGUI_PLATFORM_NAME_UPPER)
target_compile_definitions(${PROJECTNAME} PUBLIC "MYGUI_${MYGUI_PLATFORM_NAME_UPPER}_PLATFORM")

mygui_get_sdl2_targets(SDL2_LINK_TARGETS)
target_link_libraries(${PROJECTNAME}
	PUBLIC
		MyGUI::${MYGUI_PLATFORM_NAME}Platform
		MyGUI::MyGUI
		${SDL2_LINK_TARGETS}
)

if (MYGUI_INSTALL_TOOLS OR MYGUI_INSTALL_DEMOS)
	set_target_properties(${PROJECTNAME} PROPERTIES VERSION ${MYGUI_VERSION} SOVERSION "${MYGUI_VERSION_MAJOR}.${MYGUI_VERSION_MINOR}.${MYGUI_VERSION_PATCH}")
endif()
