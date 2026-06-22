#######################################################################
# This file configures package config generation, CMake export sets,
# and install rules for MyGUI.
#######################################################################

include(GNUInstallDirs)

# Create the pkg-config package files on Unix systems
if (UNIX)
  set(MYGUI_LIB_SUFFIX "")
  if (NOT BUILD_SHARED_LIBS)
    set(MYGUI_LIB_SUFFIX "${MYGUI_LIB_SUFFIX}Static")
  endif ()
  string(TOLOWER "${CMAKE_BUILD_TYPE}" MYGUI_BUILD_TYPE)
  if (MYGUI_BUILD_TYPE STREQUAL "debug")
    set(MYGUI_LIB_SUFFIX "${MYGUI_LIB_SUFFIX}_d")
  endif ()

  if (NOT BUILD_SHARED_LIBS)
    configure_file("${MYGUI_TEMPLATES_DIR}/MYGUIStatic.pc.in" "${MYGUI_BINARY_DIR}/pkgconfig/MYGUI${MYGUI_LIB_SUFFIX}.pc" @ONLY)
  else ()
    configure_file("${MYGUI_TEMPLATES_DIR}/MYGUI.pc.in" "${MYGUI_BINARY_DIR}/pkgconfig/MYGUI${MYGUI_LIB_SUFFIX}.pc" @ONLY)
  endif ()
  install(FILES "${MYGUI_BINARY_DIR}/pkgconfig/MYGUI${MYGUI_LIB_SUFFIX}.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

  # configure additional packages

endif ()

###################################################################
# Set up modern CMake package support
###################################################################

# Determine platform dependencies for the config file
set(MYGUI_CONFIG_FIND_DEPS "")
if(MYGUI_RENDERSYSTEM EQUAL 3)
	list(APPEND MYGUI_CONFIG_FIND_DEPS "find_dependency(OGRE)")
elseif(MYGUI_RENDERSYSTEM EQUAL 4 OR MYGUI_RENDERSYSTEM EQUAL 7 OR MYGUI_RENDERSYSTEM EQUAL 8)
	list(APPEND MYGUI_CONFIG_FIND_DEPS "find_dependency(OpenGL)")
endif()

string(REPLACE ";" "\n" MYGUI_CONFIG_FIND_DEPS "${MYGUI_CONFIG_FIND_DEPS}")

###################################################################
# Generate and install CMake package config files
###################################################################

include(CMakePackageConfigHelpers)
configure_package_config_file(
	"${MYGUI_TEMPLATES_DIR}/MYGUIConfig.cmake.in"
	"${MYGUI_BINARY_DIR}/MyGUIConfig.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/MyGUI"
	PATH_VARS CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR
)

configure_file(
	"${MYGUI_TEMPLATES_DIR}/MYGUIConfigVersion.cmake.in"
	"${MYGUI_BINARY_DIR}/MyGUIConfigVersion.cmake"
	@ONLY
)

export(EXPORT MyGUITargets
	NAMESPACE MyGUI::
	FILE "${MYGUI_BINARY_DIR}/MyGUITargets.cmake"
)

install(EXPORT MyGUITargets
	NAMESPACE MyGUI::
	FILE MyGUITargets.cmake
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/MyGUI"
)

install(FILES
	"${MYGUI_BINARY_DIR}/MyGUIConfig.cmake"
	"${MYGUI_BINARY_DIR}/MyGUIConfigVersion.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/MyGUI"
)

# Provide CPack packaging target
include(Packaging)
