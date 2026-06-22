#######################################################################
# This file takes care of configuring MyGUI to build with the settings
# given in CMake. It creates the necessary config.h file and will 
# also prepare package files for pkg-config and CMake.
#######################################################################

include(GNUInstallDirs)

# should we build static libs?
if (MYGUI_STATIC)
  set(MYGUI_LIB_TYPE STATIC)
else ()
  set(MYGUI_LIB_TYPE SHARED)
endif ()

if (MYGUI_STATIC)
  add_compile_definitions(MYGUI_STATIC)
endif()

# Create the pkg-config package files on Unix systems
if (UNIX)
  set(MYGUI_LIB_SUFFIX "")
  if (MYGUI_STATIC)
    set(MYGUI_LIB_SUFFIX "${MYGUI_LIB_SUFFIX}Static")
  endif ()
  string(TOLOWER "${CMAKE_BUILD_TYPE}" MYGUI_BUILD_TYPE)
  if (MYGUI_BUILD_TYPE STREQUAL "debug")
    set(MYGUI_LIB_SUFFIX "${MYGUI_LIB_SUFFIX}_d")
  endif ()

  set(MYGUI_ADDITIONAL_LIBS "")
  set(MYGUI_CFLAGS "")
  set(MYGUI_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")
  set(MYGUI_ADDITIONAL_LIBS "${MYGUI_ADDITIONAL_LIBS}")
  if (MYGUI_STATIC)
    configure_file("${MYGUI_TEMPLATES_DIR}/MYGUIStatic.pc.in" "${MYGUI_BINARY_DIR}/pkgconfig/MYGUI${MYGUI_LIB_SUFFIX}.pc" @ONLY)
  else ()
    configure_file("${MYGUI_TEMPLATES_DIR}/MYGUI.pc.in" "${MYGUI_BINARY_DIR}/pkgconfig/MYGUI${MYGUI_LIB_SUFFIX}.pc" @ONLY)
  endif ()
  install(FILES "${MYGUI_BINARY_DIR}/pkgconfig/MYGUI${MYGUI_LIB_SUFFIX}.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

  # configure additional packages
  
endif ()
