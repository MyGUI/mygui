#######################################################################
# Find all necessary and optional MYGUI dependencies
#######################################################################

# MYGUI_DEPENDENCIES_DIR can be used to specify a single base
# folder where the required dependencies may be found.
set(MYGUI_DEPENDENCIES_DIR "Dependencies" CACHE PATH "Path to prebuilt MYGUI dependencies")
include(FindPkgMacros)
getenv_path(MYGUI_DEPENDENCIES_DIR)
set(MYGUI_DEP_SEARCH_PATH
	${MYGUI_DEPENDENCIES_DIR}
	${ENV_MYGUI_DEPENDENCIES_DIR}
	"${MYGUI_BINARY_DIR}/Dependencies"
	"${MYGUI_SOURCE_DIR}/Dependencies"
	"${MYGUI_BINARY_DIR}/../Dependencies"
	"${MYGUI_SOURCE_DIR}/../Dependencies"
)

# Set hardcoded path guesses for various platforms
if(UNIX)
	set(MYGUI_DEP_SEARCH_PATH ${MYGUI_DEP_SEARCH_PATH} /usr/local)
endif()

# give guesses as hints to the find_package calls
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${MYGUI_DEP_SEARCH_PATH})
set(CMAKE_FRAMEWORK_PATH ${CMAKE_FRAMEWORK_PATH} ${MYGUI_DEP_SEARCH_PATH})

# Workaround for CMake < 3.25: marks a target's include directories as SYSTEM
# to suppress compiler warnings.
# Can be replaced with the SYSTEM keyword on FetchContent_Declare with CMake 3.25.
function(mygui_system_workaround _target)
	if(NOT TARGET ${_target})
		return()
	endif()
	get_target_property(_inc ${_target} INTERFACE_INCLUDE_DIRECTORIES)
	if(_inc)
		set_target_properties(${_target} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "")
		foreach(_dir IN LISTS _inc)
			target_include_directories(${_target} SYSTEM BEFORE PUBLIC "${_dir}")
		endforeach()
	endif()
endfunction()

#######################################################################
# Core dependencies
#######################################################################

if(MYGUI_USE_FREETYPE)
	# Find FreeType
	find_package(Freetype)
	macro_log_feature(FREETYPE_FOUND "freetype" "Portable font engine" "http://www.freetype.org" TRUE "" "")
	find_package(ZLIB)

	if(MYGUI_MSDF_FONTS)
		if(MYGUI_USE_SYSTEM_MSDFGEN)
			find_package(msdfgen REQUIRED)
		else()
			include(FetchContent)
			FetchContent_Declare(msdfgen
				GIT_REPOSITORY https://github.com/Chlumsky/msdfgen.git
				GIT_TAG v1.13
				GIT_SHALLOW TRUE
			)
			set(MSDFGEN_BUILD_STANDALONE OFF CACHE BOOL "" FORCE)
			set(MSDFGEN_DISABLE_SVG ON CACHE BOOL "" FORCE)
			set(MSDFGEN_DISABLE_PNG ON CACHE BOOL "" FORCE)
			set(MSDFGEN_USE_VCPKG OFF CACHE BOOL "" FORCE)
			set(MSDFGEN_USE_SKIA OFF CACHE BOOL "" FORCE)
			if(NOT BUILD_SHARED_LIBS)
				set(MSDFGEN_INSTALL ON CACHE BOOL "" FORCE)
			else()
				set(MSDFGEN_INSTALL OFF CACHE BOOL "" FORCE)
			endif()
			if(MSVC)
				set(MSDFGEN_DYNAMIC_RUNTIME ON CACHE BOOL "" FORCE)
			endif()
			set(MSDFGEN_BUILD_SHARED_LIBS_SAVE ${BUILD_SHARED_LIBS})
			set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
			FetchContent_MakeAvailable(msdfgen)
			mygui_system_workaround(msdfgen-core)
			mygui_system_workaround(msdfgen-ext)
			set(BUILD_SHARED_LIBS ${MSDFGEN_BUILD_SHARED_LIBS_SAVE} CACHE BOOL "" FORCE)
		endif()
	endif()
endif()

#######################################################################
# pugixml dependency
#######################################################################

if(MYGUI_USE_SYSTEM_PUGIXML)
	find_package(pugixml REQUIRED)
else()
	include(FetchContent)
	FetchContent_Declare(pugixml
		GIT_REPOSITORY https://github.com/zeux/pugixml.git
		GIT_TAG v1.16
		GIT_SHALLOW TRUE
	)
	FetchContent_MakeAvailable(pugixml)
	mygui_system_workaround(pugixml-shared)
	mygui_system_workaround(pugixml-static)
endif()

#######################################################################
# RenderSystem dependencies
#######################################################################

if(EMSCRIPTEN)
	add_library(SDL2::SDL2 INTERFACE IMPORTED GLOBAL)
	target_compile_options(SDL2::SDL2 INTERFACE "--use-port=sdl2")
	target_link_options(SDL2::SDL2 INTERFACE "--use-port=sdl2")
else()
	find_package(SDL2)
endif()

foreach(_rs IN LISTS MYGUI_BUILD_RENDERSYSTEMS)
	if(_rs EQUAL 3)
		find_package(ZLIB)
		find_package(OGRE)
		macro_log_feature(OGRE_FOUND "ogre" "Support for the Ogre render system" "" TRUE "" "")
	elseif(
		_rs EQUAL 4
		OR _rs EQUAL 7
		OR _rs EQUAL 8
	)
		if(EMSCRIPTEN)
			if(NOT TARGET SDL2_image::SDL2_image)
				add_library(SDL2_image::SDL2_image INTERFACE IMPORTED GLOBAL)
				target_compile_options(SDL2_image::SDL2_image INTERFACE "--use-port=sdl2_image:formats=png")
				target_link_options(SDL2_image::SDL2_image INTERFACE "--use-port=sdl2_image:formats=png")
			endif()
		else()
			find_package(SDL2_image)
		endif()
		find_package(OpenGL REQUIRED)
		macro_log_feature(OPENGL_FOUND "opengl" "Support for the OpenGL render system" "" TRUE "" "")
	elseif(_rs EQUAL 5)
		find_package(DirectX)
		macro_log_feature(DirectX_FOUND "DirectX" "Support for the DirectX render system" "" TRUE "" "")
	elseif(_rs EQUAL 6)
		find_package(DirectX)
		macro_log_feature(DirectX_D3D11_FOUND "DirectX11" "Support for the DirectX11 render system" "" TRUE "" "")
	elseif(_rs EQUAL 9)
		find_package(ZLIB)
		if(DEFINED OGRE_DIR AND EXISTS "${OGRE_DIR}/lib/pkgconfig/OGRE-Next.pc")
			set(ENV{PKG_CONFIG_PATH} "${OGRE_DIR}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
		endif()
		set(_saved_module_path "${CMAKE_MODULE_PATH}")
		list(PREPEND CMAKE_MODULE_PATH "${OGRE_DIR}/lib/OGRE-Next/cmake")
		find_package(OGRE)
		set(CMAKE_MODULE_PATH "${_saved_module_path}")
		unset(_saved_module_path)
		macro_log_feature(OGRE_FOUND "OGRE-Next" "Support for the OgreNext render system" "" TRUE "" "")
		if(OGRE_FOUND AND NOT TARGET OgreNext::OgreNext)
			add_library(OgreNext::OgreNext INTERFACE IMPORTED)
			set_target_properties(OgreNext::OgreNext PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES "${OGRE_INCLUDE_DIRS}"
				INTERFACE_LINK_LIBRARIES "${OGRE_LIBRARY_REL}"
			)
		endif()
	endif()
endforeach()

#######################################################################
# Demos and tools optional dependencies
#######################################################################

find_package(Doxygen)
macro_log_feature(DOXYGEN_FOUND "Doxygen" "Tool for building API documentation" "http://doxygen.org" FALSE "" "")

# Display results, terminate if anything required is missing
macro_display_feature_log()
