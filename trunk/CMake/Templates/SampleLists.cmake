######################################################################
# MYGUI SAMPLES BUILD SYSTEM
######################################################################

cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

project(MYGUI)

# Find actual MyGUI root
if (WIN32)
  get_filename_component(MYGUI_ROOT_DIR "${MYGUI_SOURCE_DIR}/../" ABSOLUTE)
  set(MYGUI_TEMPLATES_DIR "${MYGUI_ROOT_DIR}/cmake")
  set(MYGUI_WORK_DIR "${MYGUI_ROOT_DIR}")
  set(CMAKE_MODULE_PATH "${MYGUI_ROOT_DIR}/cmake")
elseif (UNIX)
  get_filename_component(MYGUI_ROOT_DIR "${MYGUI_SOURCE_DIR}/../../../" ABSOLUTE)
  set(MYGUI_TEMPLATES_DIR "${MYGUI_ROOT_DIR}/lib/MYGUI/cmake")
  set(MYGUI_WORK_DIR "${MYGUI_ROOT_DIR}")
  set(CMAKE_MODULE_PATH "${MYGUI_ROOT_DIR}/lib/MYGUI/cmake")
endif ()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  #  By default, install to the MyGUI SDK root
  SET(CMAKE_INSTALL_PREFIX
   "${MYGUI_ROOT_DIR}" CACHE PATH "MYGUI install prefix" FORCE
  )
endif ()

# Include necessary submodules
include(MyGUIConfigTargets)
set(CMAKE_PREFIX_PATH "${MYGUI_ROOT_DIR}")
set(MYGUI_INSTALL_SAMPLES TRUE)

set(MYGUI_STATIC @MYGUI_STATIC@)


#####################################################################
# Set up the basic build environment
#####################################################################

# Set compiler specific build flags
if (CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-msse)
endif ()
if (MSVC)
  add_definitions(/fp:fast)
endif ()

# Find dependencies
find_package(OGRE REQUIRED)
find_package(MYGUI REQUIRED)
find_package(OIS REQUIRED)

include_directories(
  ${OGRE_INCLUDE_DIRS}
  ${MYGUI_INCLUDE_DIRS}
  ${OIS_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/Common/include
)

if (MYGUI_STATIC)
  # need to link against all plugins
  if (MYGUI_RenderSystem_Direct3D9_FOUND)
    include_directories(${MYGUI_RenderSystem_Direct3D9_INCLUDE_DIRS})
    set(MYGUI_PLUGIN_LIBRARIES ${MYGUI_PLUGIN_LIBRARIES} ${MYGUI_RenderSystem_Direct3D9_LIBRARIES})
  endif ()
endif ()


# Configure Samples build

if (NOT OIS_FOUND)
  message(STATUS "Could not find dependency: OIS")
  message(STATUS "Skipping GUI samples")
else ()
  add_subdirectory(Demo_Colour)
  add_subdirectory(Demo_Console)
  add_subdirectory(Demo_Controllers)
  add_subdirectory(Demo_Font)
  add_subdirectory(Demo_Gui)
  add_subdirectory(Demo_ItemBox)
  add_subdirectory(Demo_PanelView)
  add_subdirectory(Demo_Picking)
  add_subdirectory(Demo_PluginStrangeButton)
  add_subdirectory(Demo_ScrollView)
  add_subdirectory(Demo_StaticImage)
  add_subdirectory(Demo_Themes)
endif ()
