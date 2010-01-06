######################################################################
# MYGUI BUILD SYSTEM
# Welcome to the CMake build system for MYGUI.
# This is the main file where we prepare the general build environment
# and provide build configuration options.
######################################################################

cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

project(MYGUI)

# Include necessary submodules
set(CMAKE_MODULE_PATH
	"${MYGUI_SOURCE_DIR}/CMake"
	"${MYGUI_SOURCE_DIR}/CMake/Utils"
	"${MYGUI_SOURCE_DIR}/CMake/Packages"
)
include(CMakeDependentOption)
include(MacroLogFeature)
include(MyGUIConfigTargets)
include(PreprocessorUtils)
set(MYGUI_TEMPLATES_DIR "${MYGUI_SOURCE_DIR}/CMake/Templates")
set(MYGUI_WORK_DIR ${MYGUI_BINARY_DIR})


#####################################################################
# Set up the basic build environment
#####################################################################

if (CMAKE_BUILD_TYPE STREQUAL "")
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
	# differentiation between debug and release builds.
	set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

# determine MyGUI version numbers
include(MyGUIGetVersion)
mygui_get_version(${MYGUI_SOURCE_DIR}/MyGUIEngine/include/MyGUI_Prerequest.h)
message(STATUS "Configuring MYGUI ${MYGUI_VERSION}")

if (NOT APPLE)
	# Create debug libraries with _d postfix
	set(CMAKE_DEBUG_POSTFIX "_d")
endif ()

# Set compiler specific build flags
if (CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-msse)
endif ()
if (MSVC)
	add_definitions(/fp:fast)
endif ()

if (CMAKE_COMPILER_IS_GNUCXX)
	# Test for GCC visibility
	include(CheckCXXCompilerFlag)
	check_cxx_compiler_flag(-fvisibility=hidden MYGUI_GCC_VISIBILITY)
	if (MYGUI_GCC_VISIBILITY)
		# determine gcc version
		execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
			OUTPUT_VARIABLE MYGUI_GCC_VERSION)
		message(STATUS "Detected g++ ${MYGUI_GCC_VERSION}")
		message(STATUS "Enabling GCC visibility flags")
		set(MYGUI_GCC_VISIBILITY_FLAGS "-DMYGUI_GCC_VISIBILITY -fvisibility=hidden")

		# check if we can safely add -fvisibility-inlines-hidden
		string(TOLOWER "${CMAKE_BUILD_TYPE}" MYGUI_BUILD_TYPE)
		if (MYGUI_BUILD_TYPE STREQUAL "debug" AND MYGUI_GCC_VERSION VERSION_LESS "4.2")
			message(STATUS "Skipping -fvisibility-inlines-hidden due to possible bug in g++ < 4.2")
		else ()
			set(MYGUI_GCC_VISIBILITY_FLAGS "${MYGUI_GCC_VISIBILITY_FLAGS} -fvisibility-inlines-hidden")
		endif ()
	endif (MYGUI_GCC_VISIBILITY)

	# Fix x64 issues on Linux
	if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" AND NOT APPLE)
		add_definitions(-fPIC)
	endif()  
endif (CMAKE_COMPILER_IS_GNUCXX)

# determine system endianess
include(TestBigEndian)
test_big_endian(MYGUI_TEST_BIG_ENDIAN)

# Add MyGUIEngine include path

# definitions for samples
set(MYGUI_LIBRARIES MyGUIEngine)

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MYGUI_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MYGUI_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MYGUI_BINARY_DIR}/bin)
if (WIN32 OR APPLE)
	if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
		# We don't want to install in default system location, install is really for the SDK, so call it that
		SET(CMAKE_INSTALL_PREFIX
		"${MYGUI_BINARY_DIR}/sdk" CACHE PATH "MYGUI install prefix" FORCE
		)
	endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
endif(WIN32 OR APPLE)

######################################################################
# Provide user options to customise the build process
######################################################################

# Customise what to build
option(MYGUI_STATIC "Static build" FALSE)
option(MYGUI_USE_FREETYPE "Use freetype for font rendering" TRUE)
option(MYGUI_DONT_USE_OBSOLETE "Remove obsole functions from build" FALSE)

set(MYGUI_RENDERSYSTEM 2 CACHE STRING
"Specify the Render System. Possible values:
  1 - Direct3D 9
  2 - Ogre
  3 - OpenGL"
)

option(MYGUI_BUILD_SAMPLES "Build MyGUI demos" TRUE)
option(MYGUI_BUILD_PLUGINS "Build MyGUI plugins" TRUE)
option(MYGUI_BUILD_TOOLS "Build the tools" TRUE)
option(MYGUI_BUILD_TESTS "Build the unit tests & TestApp" FALSE)
option(MYGUI_BUILD_WRAPPER "Build the wrapper" FALSE)

option(MYGUI_INSTALL_SAMPLES "Install MyGUI demos." FALSE)
option(MYGUI_INSTALL_TOOLS "Install MyGUI tools." FALSE)
option(MYGUI_INSTALL_DOCS "Install documentation." FALSE)
option(MYGUI_INSTALL_MEDIA "Install media files." FALSE)
option(MYGUI_INSTALL_SAMPLES_SOURCE "Install samples source files." FALSE)
cmake_dependent_option(MYGUI_INSTALL_PDB "Install debug pdb files" FALSE "MSVC" FALSE)
option(MYGUI_FULL_RPATH "Build executables with the full required RPATH to run from their install location." FALSE)
option(MYGUI_STANDALONE_BUILD "Generate build files that do not reference CMake (e.g. source distro)" FALSE)

set(MYGUI_SAMPLES_INPUT 1 CACHE STRING
"Specify the Input Manager for samples. Possible values:
  1 - OIS
  2 - Win32
  3 - Win32 (Mouse) + OIS (Keyboard)"
)

# Find dependencies
include(Dependencies)

cmake_dependent_option(MYGUI_BUILD_DOCS "Generate documentation" TRUE "DOXYGEN_FOUND" FALSE)

# global configs
include_directories(
	${MYGUI_SOURCE_DIR}/MyGUIEngine/include
)

###################################################################
# configure global build settings based on selected build options
###################################################################
include(ConfigureBuild)


##################################################################
# Now setup targets
##################################################################

# install resource files
include(InstallResources)

# Setup MyGUIEngine project
add_subdirectory(MyGUIEngine)

# Setup Platforms
add_subdirectory(Platforms)

# Setup Plugins
if (MYGUI_BUILD_PLUGINS)
	add_subdirectory(Plugins)
endif ()

if (MYGUI_BUILD_SAMPLES)
	add_subdirectory(Demos)
endif ()

# Setup command-line tools
if (MYGUI_BUILD_TOOLS)
	add_subdirectory(Tools)
endif ()

# Setup tests
if (MYGUI_BUILD_TESTS)
	add_subdirectory(UnitTests)
endif ()

# Setup wrapers
if (MYGUI_BUILD_WRAPPER)
	add_subdirectory(Wrapper)
endif ()

# Install documentation
if (MYGUI_BUILD_DOCS)
	add_subdirectory(Docs)
endif ()

# Install media files
#if (MYGUI_INSTALL_MEDIA)
#	add_subdirectory(Media)
#endif ()

# Install CMake modules
add_subdirectory(CMake)

# Provide CPack packaging target
include(Packaging)
