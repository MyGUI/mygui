#######################################################################
# This file takes care of configuring Ogre to build with the settings
# given in CMake. It creates the necessary config.h file and will 
# also prepare package files for pkg-config and CMake.
#######################################################################

# should we build static libs?
if (MYGUI_STATIC)
  set(MYGUI_LIB_TYPE STATIC)
else ()
  set(MYGUI_LIB_TYPE SHARED)
endif ()

set(MYGUI_STATIC_LIB 0)


if (MYGUI_STATIC)
  set(MYGUI_STATIC_LIB 1)
endif()
add_definitions(-DHAVE_MYGUI_BUILDSETTINGS_H)

# generate buildsettings.h 
#configure_file(${MYGUI_TEMPLATES_DIR}/buildsettings.h.in ${MYGUI_BINARY_DIR}/include/buildsettings.h @ONLY)
#install(FILES ${MYGUI_BINARY_DIR}/include/buildsettings.h DESTINATION include/MYGUI)

# Read contents of the OgreConfig.h file
#file(READ "${MYGUI_SOURCE_DIR}/OgreMain/include/OgreConfig.h" MYGUI_CONFIG_H)
# add HAVE_MYGUI_BUILDSETTINGS_H preprocessor define
#file(WRITE ${MYGUI_BINARY_DIR}/include/OgreConfig.h "#define HAVE_MYGUI_BUILDSETTINGS_H\n${MYGUI_CONFIG_H}")
#install(FILES ${MYGUI_BINARY_DIR}/include/OgreConfig.h DESTINATION include/MYGUI)


# Create the pkg-config package files on Unix systems
if (UNIX)
  set(MYGUI_LIB_SUFFIX "")
  set(MYGUI_PLUGIN_PREFIX "")
  set(MYGUI_PLUGIN_EXT ".so")
  if (MYGUI_STATIC)
    set(MYGUI_LIB_SUFFIX "${MYGUI_LIB_SUFFIX}Static")
    set(MYGUI_PLUGIN_PREFIX "lib")
    set(MYGUI_PLUGIN_EXT ".a")
  endif ()
  string(TOLOWER "${CMAKE_BUILD_TYPE}" MYGUI_BUILD_TYPE)
  if (MYGUI_BUILD_TYPE STREQUAL "debug")
    set(MYGUI_LIB_SUFFIX "${MYGUI_LIB_SUFFIX}_d")
  endif ()

  set(MYGUI_ADDITIONAL_LIBS "")
  set(MYGUI_CFLAGS "")
  set(MYGUI_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})
  set(MYGUI_ADDITIONAL_LIBS "${MYGUI_ADDITIONAL_LIBS} -luuid")
  if (MYGUI_STATIC)
    configure_file(${MYGUI_TEMPLATES_DIR}/MYGUIStatic.pc.in ${MYGUI_BINARY_DIR}/pkgconfig/MYGUI${MYGUI_LIB_SUFFIX}.pc @ONLY)
  else ()
    configure_file(${MYGUI_TEMPLATES_DIR}/MYGUI.pc.in ${MYGUI_BINARY_DIR}/pkgconfig/MYGUI${MYGUI_LIB_SUFFIX}.pc @ONLY)
  endif ()
  install(FILES ${MYGUI_BINARY_DIR}/pkgconfig/MYGUI${MYGUI_LIB_SUFFIX}.pc DESTINATION lib/pkgconfig)

  # configure additional packages
  
endif ()

if (MYGUI_STANDALONE_BUILD)
  set(CMAKE_USE_RELATIVE_PATHS true)
  set(CMAKE_SUPPRESS_REGENERATION true)
endif()
