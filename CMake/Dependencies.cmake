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
if (UNIX)
  set(MYGUI_DEP_SEARCH_PATH ${MYGUI_DEP_SEARCH_PATH} /usr/local)
endif ()

# give guesses as hints to the find_package calls
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${MYGUI_DEP_SEARCH_PATH})
set(CMAKE_FRAMEWORK_PATH ${CMAKE_FRAMEWORK_PATH} ${MYGUI_DEP_SEARCH_PATH})

#######################################################################
# Core dependencies
#######################################################################

if (MYGUI_USE_FREETYPE)
	# Find FreeType
	find_package(Freetype)
	macro_log_feature(FREETYPE_FOUND "freetype" "Portable font engine" "http://www.freetype.org" TRUE "" "")
	find_package(ZLIB)

	if (MYGUI_MSDF_FONTS)
		if (MYGUI_USE_SYSTEM_MSDFGEN)
			find_package(msdfgen)
		else()
			include(FetchContent)
			FetchContent_Declare(msdfgen
				GIT_REPOSITORY https://github.com/Chlumsky/msdfgen.git
				GIT_TAG v1.13
				GIT_SHALLOW TRUE
				SYSTEM
			)
			set(MSDFGEN_BUILD_STANDALONE OFF CACHE BOOL "" FORCE)
			set(MSDFGEN_DISABLE_SVG ON CACHE BOOL "" FORCE)
			set(MSDFGEN_DISABLE_PNG ON CACHE BOOL "" FORCE)
			set(MSDFGEN_USE_VCPKG OFF CACHE BOOL "" FORCE)
			set(MSDFGEN_USE_SKIA OFF CACHE BOOL "" FORCE)
			set(MSDFGEN_INSTALL OFF CACHE BOOL "" FORCE)
			set(MSDFGEN_BUILD_SHARED_LIBS_SAVE ${BUILD_SHARED_LIBS})
			set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
			FetchContent_MakeAvailable(msdfgen)
			set(BUILD_SHARED_LIBS ${MSDFGEN_BUILD_SHARED_LIBS_SAVE} CACHE BOOL "" FORCE)
		endif()
	endif()
endif()

#######################################################################
# pugixml dependency
#######################################################################

if (MYGUI_USE_SYSTEM_PUGIXML)
	find_package(pugixml REQUIRED)
else()
	include(FetchContent)
	FetchContent_Declare(pugixml
		GIT_REPOSITORY https://github.com/zeux/pugixml.git
		GIT_TAG v1.16
		GIT_SHALLOW TRUE
		SYSTEM
	)
	FetchContent_MakeAvailable(pugixml)
endif()

#######################################################################
# RenderSystem dependencies
#######################################################################

find_package(SDL2)

if(MYGUI_RENDERSYSTEM EQUAL 3)
	find_package(ZLIB)
	find_package(OGRE)
	macro_log_feature(OGRE_FOUND "ogre" "Support for the Ogre render system" "" TRUE "" "")
elseif(MYGUI_RENDERSYSTEM EQUAL 4 OR MYGUI_RENDERSYSTEM EQUAL 7)
	find_package(SDL2_image)
	find_package(OpenGL REQUIRED)
	macro_log_feature(OPENGL_FOUND "opengl" "Support for the OpenGL render system" "" TRUE "" "")
elseif(MYGUI_RENDERSYSTEM EQUAL 5)
	find_package(DirectX)
	macro_log_feature(DirectX_FOUND "DirectX" "Support for the DirectX render system" "" TRUE "" "")
elseif(MYGUI_RENDERSYSTEM EQUAL 6)
	find_package(DirectX)
	macro_log_feature(DirectX_FOUND "DirectX11" "Support for the DirectX11 render system" "" TRUE "" "")
#elseif for RENDERSYSTEM 7 is covered with RENDERSYSTEM 4
endif()

#######################################################################
# Demos and tools optional dependencies
#######################################################################

find_package(Doxygen)
macro_log_feature(DOXYGEN_FOUND "Doxygen" "Tool for building API documentation" "http://doxygen.org" FALSE "" "")


# Display results, terminate if anything required is missing
MACRO_DISPLAY_FEATURE_LOG()
