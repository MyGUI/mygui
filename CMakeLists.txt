cmake_minimum_required(VERSION 3.22)

project(MYGUI)

list(APPEND CMAKE_MODULE_PATH
	"${MYGUI_SOURCE_DIR}/CMake"
	"${MYGUI_SOURCE_DIR}/CMake/Utils"
	"${MYGUI_SOURCE_DIR}/CMake/Packages"
)

include(CMakeDependentOption)
include(MacroLogFeature)
include(MyGUIConfigTargets)
set(MYGUI_TEMPLATES_DIR "${MYGUI_SOURCE_DIR}/CMake/Templates")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

######################################################################
# Provide user options to customise the build process
######################################################################

if(EMSCRIPTEN)
	set(BUILD_SHARED_LIBS OFF)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# Customise what to build
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(MYGUI_STATIC "Static build" FALSE)
if(MYGUI_STATIC)
	message(DEPRECATION
		"MYGUI_STATIC is deprecated. Use BUILD_SHARED_LIBS=OFF instead."
	)
	set(BUILD_SHARED_LIBS OFF)
endif()
option(MYGUI_USE_FREETYPE "Use freetype for font texture rendering" TRUE)
option(MYGUI_MSDF_FONTS "Enable msdf fonts generation support" FALSE)
option(MYGUI_USE_SYSTEM_MSDFGEN FALSE)
option(MYGUI_USE_SYSTEM_PUGIXML "Use system installed pugixml" FALSE)
option(MYGUI_DONT_USE_OBSOLETE "Remove obsolete functions from build" FALSE)

set(MYGUI_RENDERSYSTEM 7 CACHE STRING
"Specify the Render System. Possible values:
  1 - Dummy
  2 - Export (not used)
  3 - Ogre
  4 - OpenGL
  5 - Direct3D 9
  6 - Direct3D 11
  7 - OpenGL 3.x
  8 - OpenGL ES 2.0 (Emscripten)
  9 - OgreNext"
)

option(MYGUI_BUILD_DEMOS "Build MyGUI demos" TRUE)
option(MYGUI_BUILD_TOOLS "Build the tools" TRUE)
option(MYGUI_BUILD_UNITTESTS "Build the unit tests" FALSE)
option(MYGUI_BUILD_TEST_APP "Build TestApp" FALSE)
cmake_dependent_option(MYGUI_BUILD_WRAPPER "Build the wrapper" FALSE "WIN32" FALSE)

cmake_dependent_option(MYGUI_INSTALL_DEMOS "Install MyGUI demos." FALSE "MYGUI_BUILD_DEMOS" FALSE)
cmake_dependent_option(MYGUI_INSTALL_TOOLS "Install MyGUI tools." FALSE "MYGUI_BUILD_TOOLS" FALSE)
option(MYGUI_INSTALL_DOCS "Install documentation." FALSE)
cmake_dependent_option(MYGUI_INSTALL_PDB "Install debug pdb files" FALSE "MSVC;BUILD_SHARED_LIBS" FALSE)

### Internal options ###

# Used to not annoy users with high level warnings (should be TRUE for developers)
option(MYGUI_HIGH_LEVEL_WARNINGS "Use high level compiler warnings. Developers should enable this." FALSE)
option(MYGUI_CLANG_TIDY_BUILD "Hack some things for clang-tidy runs, f.e. disable PCH" FALSE)
mark_as_advanced(
	MYGUI_HIGH_LEVEL_WARNINGS
	MYGUI_CLANG_TIDY_BUILD
)

#####################################################################
# Set up the basic build environment
#####################################################################

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

# determine MyGUI version numbers
include(MyGUIGetVersion)
mygui_get_version("${MYGUI_SOURCE_DIR}/MyGUIEngine/include/MyGUI_Prerequest.h")
message(STATUS "Configuring MYGUI ${MYGUI_VERSION}")

if(NOT APPLE)
	# Create debug libraries with _d postfix
	set(CMAKE_DEBUG_POSTFIX "_d")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")

if(MSVC)
	option(MYGUI_USE_PROJECT_FOLDERS "Use Visual Studio solution folders for projects." TRUE)
endif()

if(MYGUI_USE_PROJECT_FOLDERS)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

######################################################################
# Dependencies and docs option
######################################################################

include(Dependencies)

cmake_dependent_option(MYGUI_BUILD_DOCS "Generate documentation" TRUE "DOXYGEN_FOUND" FALSE)

######################################################################
# Compiler toolchain settings
######################################################################

include(CompilerWarnings)
include(Sanitizers)

##################################################################
# Setup targets
##################################################################

add_subdirectory(MyGUIEngine)
add_subdirectory(Platforms)

if(MYGUI_BUILD_DEMOS OR MYGUI_BUILD_TOOLS OR MYGUI_BUILD_UNITTESTS OR MYGUI_BUILD_TEST_APP)
	add_subdirectory(Common)
endif()

if(MYGUI_BUILD_DEMOS)
	add_subdirectory(Demos)
endif()

if(MYGUI_BUILD_TOOLS)
	add_subdirectory(Tools)
endif()

if(MYGUI_BUILD_UNITTESTS OR MYGUI_BUILD_TEST_APP)
	if(MYGUI_BUILD_UNITTESTS)
		enable_testing()
	endif()
	add_subdirectory(UnitTests)
endif()

if(MYGUI_BUILD_WRAPPER)
	add_subdirectory(Wrappers)
endif()

if(MYGUI_BUILD_DOCS)
	add_subdirectory(Docs)
endif()

###################################################################
# Configure install rules, package configs, and export sets
###################################################################
add_subdirectory(Media)
include(InstallRules)
