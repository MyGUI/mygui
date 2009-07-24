##################################################################
# Generate and install the config files needed for the samples
##################################################################

if (WIN32)
  set(MYGUI_MEDIA_PATH "media")
  set(MYGUI_MEDIA_DIR_REL "../../${MYGUI_MEDIA_PATH}")
  set(MYGUI_MEDIA_DIR_DBG "../../${MYGUI_MEDIA_PATH}")
  set(MYGUI_PLUGIN_DIR_REL ".")
  set(MYGUI_PLUGIN_DIR_DBG ".")
elseif (UNIX)
  set(MYGUI_MEDIA_PATH "share/MYGUI/media")
  set(MYGUI_MEDIA_DIR_REL "../${MYGUI_MEDIA_PATH}")
  set(MYGUI_MEDIA_DIR_DBG "../../${MYGUI_MEDIA_PATH}")
  set(MYGUI_PLUGIN_DIR_REL "../lib/MYGUI")
  set(MYGUI_PLUGIN_DIR_DBG "../../lib/MYGUI")
endif ()

configure_file(${MYGUI_TEMPLATES_DIR}/resources.cfg.in ${MYGUI_BINARY_DIR}/bin/debug/resources.cfg)
configure_file(${MYGUI_TEMPLATES_DIR}/resources.cfg.in ${MYGUI_BINARY_DIR}/bin/release/resources.cfg)
# create plugins.cfg
configure_file(${MYGUI_TEMPLATES_DIR}/plugins_d.cfg.in ${MYGUI_BINARY_DIR}/bin/debug/plugins.cfg)
configure_file(${MYGUI_TEMPLATES_DIR}/plugins.cfg.in ${MYGUI_BINARY_DIR}/bin/release/plugins.cfg)

# install resource files
if (MYGUI_INSTALL_SAMPLES OR MYGUI_INSTALL_TOOLS)
  install(FILES 
    ${MYGUI_BINARY_DIR}/bin/debug/resources.cfg
    ${MYGUI_BINARY_DIR}/bin/debug/plugins.cfg
    DESTINATION "bin${MYGUI_DEBUG_PATH}" CONFIGURATIONS Debug
  )
  install(FILES 
    ${MYGUI_BINARY_DIR}/bin/release/resources.cfg
    ${MYGUI_BINARY_DIR}/bin/release/plugins.cfg
    DESTINATION "bin${MYGUI_RELEASE_PATH}" CONFIGURATIONS Release None ""
  )
  install(FILES 
    ${MYGUI_BINARY_DIR}/bin/release/resources.cfg
    ${MYGUI_BINARY_DIR}/bin/release/plugins.cfg
    DESTINATION "bin${MYGUI_RELWDBG_PATH}" CONFIGURATIONS RelWithDebInfo
  )
  install(FILES 
    ${MYGUI_BINARY_DIR}/bin/release/resources.cfg
    ${MYGUI_BINARY_DIR}/bin/release/plugins.cfg
    DESTINATION "bin${MYGUI_MINSIZE_PATH}" CONFIGURATIONS MinSizeRel
  )
endif ()

if (WIN32)
  # create resources.cfg
  configure_file(${MYGUI_TEMPLATES_DIR}/resources.cfg.in ${MYGUI_BINARY_DIR}/bin/debug/resources.cfg)
  configure_file(${MYGUI_TEMPLATES_DIR}/resources.cfg.in ${MYGUI_BINARY_DIR}/bin/release/resources.cfg)
  configure_file(${MYGUI_TEMPLATES_DIR}/resources.cfg.in ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/resources.cfg)
  configure_file(${MYGUI_TEMPLATES_DIR}/resources.cfg.in ${MYGUI_BINARY_DIR}/bin/minsizerel/resources.cfg)
  # create plugins.cfg
  configure_file(${MYGUI_TEMPLATES_DIR}/plugins_d.cfg.in ${MYGUI_BINARY_DIR}/bin/debug/plugins_d.cfg)
  configure_file(${MYGUI_TEMPLATES_DIR}/plugins.cfg.in ${MYGUI_BINARY_DIR}/bin/release/plugins.cfg)
  configure_file(${MYGUI_TEMPLATES_DIR}/plugins.cfg.in ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/plugins.cfg)
  configure_file(${MYGUI_TEMPLATES_DIR}/plugins.cfg.in ${MYGUI_BINARY_DIR}/bin/minsizerel/plugins.cfg)
else() # other OS only need one cfg file
  string(TOLOWER "${CMAKE_BUILD_TYPE}" MYGUI_BUILD_TYPE)
  if (MYGUI_BUILD_TYPE STREQUAL "debug" AND NOT APPLE)
    set(MYGUI_CFG_SUFFIX "_d")
  endif ()
  # create resources.cfg
  configure_file(${MYGUI_TEMPLATES_DIR}/resources.cfg.in ${MYGUI_BINARY_DIR}/bin/resources.cfg)
  # create plugins.cfg
  configure_file(${MYGUI_TEMPLATES_DIR}/plugins${MYGUI_CFG_SUFFIX}.cfg.in ${MYGUI_BINARY_DIR}/bin/plugins.cfg)
endif ()

