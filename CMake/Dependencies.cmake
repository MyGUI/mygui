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

if (WIN32)
	include(MyGUIInstallDependencies)
endif ()


#######################################################################
# Core dependencies
#######################################################################

if (MYGUI_USE_FREETYPE)
	# Find FreeType
	find_package(Freetype)
	macro_log_feature(FREETYPE_FOUND "freetype" "Portable font engine" "http://www.freetype.org" TRUE "" "")
endif()

#######################################################################
# RenderSystem dependencies
#######################################################################

if(MYGUI_RENDERSYSTEM EQUAL 5)
	# Find DirectX
	if(WIN32)
		find_package(DirectX)
		macro_log_feature(DirectX_FOUND "DirectX" "Support for the DirectX render system" "http://msdn.microsoft.com/en-us/directx/" TRUE "" "")
	endif()
elseif(MYGUI_RENDERSYSTEM EQUAL 3)
	# Find OGRE
	find_package(OGRE_Old)
	macro_log_feature(OGRE_FOUND "ogre" "Support for the Ogre render system" "" TRUE "" "")
elseif(MYGUI_RENDERSYSTEM EQUAL 4)
	#find_package(ZLIB)
	#macro_log_feature(ZLIB_FOUND "zlib" "Simple data compression library" "http://www.zlib.net" TRUE "" "")
	#find_package(PNG)
	#macro_log_feature(PNG_FOUND "PNG" "Png image codec" "http://www.libpng.org/" TRUE "" "")
	find_package(OpenGL)
	macro_log_feature(OPENGL_FOUND "opengl" "Support for the OpenGL render system" "" TRUE "" "")
elseif(MYGUI_RENDERSYSTEM EQUAL 6)
	# Find DirectX11
	if(WIN32)
		find_package(DirectX11)
		macro_log_feature(DirectX_FOUND "DirectX11" "Support for the DirectX11 render system" "http://msdn.microsoft.com/en-us/directx/" TRUE "" "")
	endif()
endif()

#######################################################################
# Demos and tools optional dependencies
#######################################################################

# Find OIS
if (MYGUI_SAMPLES_INPUT EQUAL 1)
	find_package(OIS)
	macro_log_feature(OIS_FOUND "OIS" "Input library needed for the samples" "http://sourceforge.net/projects/wgois" FALSE "" "")
elseif (MYGUI_SAMPLES_INPUT EQUAL 3)
	find_package(OIS)
	macro_log_feature(OIS_FOUND "OIS" "Input library needed for the samples" "http://sourceforge.net/projects/wgois" FALSE "" "")
endif()

#######################################################################
# Tools
#######################################################################

find_package(Doxygen)
macro_log_feature(DOXYGEN_FOUND "Doxygen" "Tool for building API documentation" "http://doxygen.org" FALSE "" "")


#######################################################################
# Apple-specific
#######################################################################
if (APPLE)
  find_package(Carbon)
  macro_log_feature(Carbon_FOUND "Carbon" "Carbon" "http://www.apple.com" TRUE "" "")

  find_package(Cocoa)
  macro_log_feature(Cocoa_FOUND "Cocoa" "Cocoa" "http://www.apple.com" TRUE "" "")

  find_package(IOKit)
  macro_log_feature(IOKit_FOUND "IOKit" "IOKit HID framework needed by the samples" "http://www.apple.com" FALSE "" "")
endif(APPLE)

# Display results, terminate if anything required is missing
MACRO_DISPLAY_FEATURE_LOG()
