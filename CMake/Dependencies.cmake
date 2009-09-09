#######################################################################
# Find all necessary and optional MYGUI dependencies
#######################################################################

# MYGUI_DEPENDENCIES_DIR can be used to specify a single base
# folder where the required dependencies may be found.
set(MYGUI_DEPENDENCIES_DIR "" CACHE PATH "Path to prebuilt MYGUI dependencies")
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

include(MyGUIInstallDependencies)


#######################################################################
# Core dependencies
#######################################################################

# Find FreeType
find_package(Freetype)
macro_log_feature(FREETYPE_FOUND "freetype" "Portable font engine" "http://www.freetype.org" TRUE "" "")

#######################################################################
# RenderSystem dependencies
#######################################################################

if(MYGUI_RENDERSYSTEM EQUAL 1)
	# Find DirectX
	if(WIN32)
		find_package(DirectX)
		macro_log_feature(DirectX_FOUND "DirectX" "Support for the DirectX render system" "http://msdn.microsoft.com/en-us/directx/" FALSE "" "")
	endif()
elseif(MYGUI_RENDERSYSTEM EQUAL 2)
	# Find OGRE
	find_package(OGRE)
	macro_log_feature(OGRE_FOUND "ogre" "Support for the Ogre render system" "" FALSE "" "")
elseif(MYGUI_RENDERSYSTEM EQUAL 3)
	find_package(OpenGL)
	macro_log_feature(OPENGL_FOUND "opengl" "Support for the OpenGL render system" "" FALSE "" "")
endif()


# Find OIS
find_package(OIS)
macro_log_feature(OIS_FOUND "OIS" "Input library needed for the samples" "http://sourceforge.net/projects/wgois" FALSE "" "")

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
