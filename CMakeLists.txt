######################################################################
# MYGUI BUILD SYSTEM
# Welcome to the CMake build system for MYGUI.
# This is the main file where we prepare the general build environment
# and provide build configuration options.
######################################################################

cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0011 NEW)

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
include(Utils)
set(MYGUI_TEMPLATES_DIR "${MYGUI_SOURCE_DIR}/CMake/Templates")
set(MYGUI_WORK_DIR ${MYGUI_BINARY_DIR})

SET(CMAKE_CXX_STANDARD 11)

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

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "Intel")
	# Test for GCC visibility
	include(CheckCXXCompilerFlag)
	check_cxx_compiler_flag(-fvisibility=hidden MYGUI_GCC_VISIBILITY)
	if (MYGUI_GCC_VISIBILITY)
		# determine gcc version
		execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE MYGUI_GCC_VERSION)
		message(STATUS "Detected g++ ${MYGUI_GCC_VERSION}")
		message(STATUS "Enabling GCC visibility flags")
		set(MYGUI_GCC_VISIBILITY_FLAGS "-DMYGUI_GCC_VISIBILITY -fvisibility=hidden")

		# check if we can safely add -fvisibility-inlines-hidden
		string(TOLOWER "${CMAKE_BUILD_TYPE}" MYGUI_BUILD_TYPE)
		if (MYGUI_BUILD_TYPE STREQUAL "debug")
			if(MYGUI_GCC_VERSION VERSION_LESS "4.2")
				message(STATUS "Skipping -fvisibility-inlines-hidden due to possible bug in g++ < 4.2")
			else() # double if because of bug in CMake 2.6
				set(MYGUI_GCC_VISIBILITY_FLAGS "${MYGUI_GCC_VISIBILITY_FLAGS} -fvisibility-inlines-hidden")
			endif()
		else ()
			set(MYGUI_GCC_VISIBILITY_FLAGS "${MYGUI_GCC_VISIBILITY_FLAGS} -fvisibility-inlines-hidden")
		endif ()
	endif (MYGUI_GCC_VISIBILITY)

	# Fix x64 issues on Linux
	if(("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" OR "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "amd64") AND NOT APPLE)
		add_definitions(-fPIC)
	endif()
endif ()

# definitions for samples
set(MYGUI_LIBRARIES MyGUIEngine)

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
if (WIN32 OR APPLE)
	if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
		if (UNIX)
			# On Mac OS X and Linux, standard behavior is to install to /usr/local/lib
			set(CMAKE_INSTALL_PREFIX "/usr/local")
		elseif (WIN32)
			# On Windows, we don't want to install in default system location, install is really for the SDK, so call it that
			SET(CMAKE_INSTALL_PREFIX
				"${MYGUI_BINARY_DIR}/sdk" CACHE PATH "MYGUI install prefix" FORCE
			)
		endif ()
	endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
endif(WIN32 OR APPLE)

######################################################################
# Provide user options to customise the build process
######################################################################

if (EMSCRIPTEN)
	set(MYGUI_STATIC True CACHE BOOL "")
	set(MYGUI_DISABLE_PLUGINS True CACHE BOOL "")
	set(MYGUI_USE_SYSTEM_GLEW True CACHE BOOL "")

	set(OPENGL_gl_LIBRARY Dummy CACHE STRING "")
	set(OPENGL_glu_LIBRARY Dummy CACHE STRING "")
	set(GLEW_LIBRARY Dummy CACHE STRING "")
	set(SDL2_LIBRARY Dummy CACHE STRING "")
	set(SDL2_IMAGE_LIBRARY Dummy CACHE STRING "")
	set(SDL2_INCLUDE_DIR "" CACHE STRING "")
	set(SDL2_IMAGE_INCLUDE_DIR "" CACHE STRING "")

	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--std=c++11 -O2 --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/Media/@/ -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]'")
endif ()

# Customise what to build
option(MYGUI_STATIC "Static build" FALSE)
option(MYGUI_DISABLE_PLUGINS "Disable plugins support" FALSE)
option(MYGUI_USE_FREETYPE "Use freetype for font rendering" TRUE)
option(MYGUI_DONT_USE_OBSOLETE "Remove obsole functions from build" FALSE)

set(MYGUI_RENDERSYSTEM 3 CACHE STRING
"Specify the Render System. Possible values:
  1 - Dummy
  2 - Export (latter)
  3 - Ogre
  4 - OpenGL
  5 - Direct3D 9
  6 - Direct3D 11
  7 - OpenGL 3.x
  8 - OpenGL ES 2.0 (Emscripten)"
)

if(MYGUI_RENDERSYSTEM EQUAL 4 OR MYGUI_RENDERSYSTEM EQUAL 7)
	option(MYGUI_USE_SYSTEM_GLEW "Use system OpenGL Extension Wrangler Library (GLEW)" FALSE)
endif()

option(MYGUI_BUILD_DEMOS "Build MyGUI demos" TRUE)
option(MYGUI_BUILD_PLUGINS "Build MyGUI plugins" TRUE)
option(MYGUI_BUILD_TOOLS "Build the tools" TRUE)
option(MYGUI_BUILD_UNITTESTS "Build the unit tests" FALSE)
option(MYGUI_BUILD_TEST_APP "Build TestApp" FALSE)
option(MYGUI_BUILD_WRAPPER "Build the wrapper" FALSE)

option(MYGUI_INSTALL_SAMPLES "Install MyGUI demos." FALSE)
option(MYGUI_INSTALL_TOOLS "Install MyGUI tools." FALSE)
option(MYGUI_INSTALL_DOCS "Install documentation." FALSE)
option(MYGUI_INSTALL_MEDIA "Install media files." FALSE)
option(MYGUI_INSTALL_SAMPLES_SOURCE "Install samples source files." FALSE)
cmake_dependent_option(MYGUI_INSTALL_PDB "Install debug pdb files" FALSE "MSVC" FALSE)
option(MYGUI_FULL_RPATH "Build executables with the full required RPATH to run from their install location." FALSE)
option(MYGUI_STANDALONE_BUILD "Generate build files that do not reference CMake (should be used only from Scripts/prepareRelease.py)" FALSE)
MARK_AS_ADVANCED(MYGUI_STANDALONE_BUILD)
option(MYGUI_GENERATE_LIST_FILES_FROM_VSPROJECT "Generate .list files from visual studio project files" FALSE)

if (MSVC)
	option(MYGUI_USE_PROJECT_FOLDERS "Use Visual Studio solution folders for projects." FALSE)
endif ()

# Used to not annoy users with high level warnings (should be TRUE for developers)
option(MYGUI_HIGH_LEVEL_WARNINGS "Use high level compiler warnings. Developers should enable this." FALSE)
if (MSVC)
	option(MYGUI_CHECK_MEMORY_LEAKS "Check memory leaks. Developers should enable this." FALSE)
endif ()

if (MYGUI_USE_PROJECT_FOLDERS)
	SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
endif ()

# Global defines
if (MYGUI_CHECK_MEMORY_LEAKS)
	add_definitions(-DMYGUI_CHECK_MEMORY_LEAKS)
endif ()

if (MYGUI_DONT_USE_OBSOLETE)
	add_definitions(-DMYGUI_DONT_USE_OBSOLETE)
endif ()
# End of Global defines

# Find dependencies
include(Dependencies)

# Set compiler specific build flags
if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "Intel")
	if (NOT MYGUI_HIGH_LEVEL_WARNINGS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
	else ()
		if (MYGUI_USE_FREETYPE)
			ignore_warnings(${FREETYPE_INCLUDE_DIRS})
			ignore_warnings(${FREETYPE_FT2BUILD_INCLUDE_DIR})
		endif ()
		if (MYGUI_RENDERSYSTEM EQUAL 3)
			# to avoid warnings from OGRE
			ignore_warnings(${OGRE_INCLUDE_DIR})
		endif ()

		if (MYGUI_BUILD_DEMOS OR MYGUI_BUILD_TOOLS OR MYGUI_BUILD_UNITTESTS OR MYGUI_BUILD_TEST_APP)
			ignore_warnings(${SDL2_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS}/..)
		endif ()

		if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
                        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
                        # useful, TODO
                        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")
                        # might be useful
                        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exceptions -Wno-documentation-unknown-command -Wno-unknown-warning-option -Wno-sign-conversion -Wno-conversion -Wno-documentation -Wno-old-style-cast")
                        # not useful
                        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-prototypes -Wno-reorder -Wno-undefined-var-template -Wno-c++98-compat-pedantic -Wno-c++98-compat -Wno-c++11-extensions -Wno-float-equal -Wno-padded -Wno-padded -Wno-weak-vtables -Wno-duplicate-enum -Wno-exit-time-destructors -Wno-unused-parameter -Wno-global-constructors")
		else ()
			# very interesting option, but way too many warnings
			#add_definitions(-Weffc++)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wall -Wno-reorder -Winit-self -Woverloaded-virtual -Wcast-qual -Wwrite-strings -Wextra -Wno-unused-parameter")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
			if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Intel")
				set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wctor-dtor-privacy")
				set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-show-option")
			endif ()

			# MyGUI_UString.h ignored from warnings because of this
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
		endif ()
	endif ()
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	# usage:
	# UBSAN_OPTIONS=print_stacktrace=1,suppressions=sanitizers/UBSanitizer.supp <command>
	option(MYGUI_UB_SANITIZER "Compile with undefined behaviour sanitizer" FALSE)
	if (MYGUI_UB_SANITIZER)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fno-optimize-sibling-calls")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fno-sanitize-recover=undefined")
	endif()

	# usage:
	# ASAN_OPTIONS=alloc_dealloc_mismatch=0,check_initialization_order=1,detect_stack_use_after_return=1 LSAN_OPTIONS=suppressions=sanitizers/LeaksSanitizer.supp <command>
	# alloc_dealloc_mismatch is here due to issues with Ogre and no clear way to suppress the issue
	option(MYGUI_ADDRESS_SANITIZER "Compile with address sanitizer" FALSE)
	if (MYGUI_ADDRESS_SANITIZER)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fno-optimize-sibling-calls")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize-address-use-after-scope")
	endif()
endif ()

if (MSVC)
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")

	# Use the highest warning level for visual studio.
	if (MYGUI_HIGH_LEVEL_WARNINGS)
		SET(WARNING_LEVEL "/W4")
	else ()
		SET(WARNING_LEVEL "/W3")
	endif ()
	# disable: warning C4100: '***' : unreferenced formal parameter
	add_definitions(/wd4100)
	# disable: warning C4127: conditional expression is constant
	add_definitions(/wd4127)
	# disable: warning C4512: '***' : assignment operator could not be generated
	add_definitions(/wd4512)
	# disable: warning C4275: non dll-interface class '***' used as base for dll-interface clas '***'
	add_definitions(/wd4275)

	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		STRING(REGEX REPLACE "/W[0-4]" "${WARNING_LEVEL}" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else ()
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_LEVEL}")
	endif ()

endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
	# Otherwise, boost complains that there is no threading support.
	SET(CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}")
endif ()

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

if (MYGUI_BUILD_DEMOS OR MYGUI_BUILD_TOOLS OR MYGUI_BUILD_UNITTESTS OR MYGUI_BUILD_TEST_APP)
	add_subdirectory(Common)
endif ()

# Setup Plugins
if (MYGUI_BUILD_PLUGINS)
	add_subdirectory(Plugins)
endif ()

if (MYGUI_BUILD_DEMOS)
	add_subdirectory(Demos)
endif ()

# Setup command-line tools
if (MYGUI_BUILD_TOOLS)
	add_subdirectory(Tools)
endif ()

# Setup tests
if (MYGUI_BUILD_UNITTESTS OR MYGUI_BUILD_TEST_APP)
	add_subdirectory(UnitTests)
endif ()

# Setup wrapers
if (MYGUI_BUILD_WRAPPER)
	add_subdirectory(Wrappers)
endif ()

# Install documentation
if (MYGUI_BUILD_DOCS)
	add_subdirectory(Docs)
endif ()

# Install media files
if (MYGUI_INSTALL_MEDIA)
	add_subdirectory(Media)
endif ()

# Install CMake modules
add_subdirectory(CMake)

# Provide CPack packaging target
include(Packaging)
