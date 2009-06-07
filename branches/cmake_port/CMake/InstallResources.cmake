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

# configure plugins.cfg
if (NOT MYGUI_BUILD_RENDERSYSTEM_D3D9)
  set(MYGUI_COMMENT_RENDERSYSTEM_D3D9 "#")
endif ()
if (NOT MYGUI_BUILD_RENDERSYSTEM_D3D10)
  set(MYGUI_COMMENT_RENDERSYSTEM_D3D10 "#")
endif ()
if (CMAKE_SYSTEM_VERSION VERSION_LESS "6.0")
  set(MYGUI_COMMENT_RENDERSYSTEM_D3D10 "#")
endif ()
if (NOT MYGUI_BUILD_RENDERSYSTEM_GL)
  set(MYGUI_COMMENT_RENDERSYSTEM_GL "#")
endif ()
if (NOT MYGUI_BUILD_PLUGIN_BSP)
  set(MYGUI_COMMENT_PLUGIN_BSP "#")
endif ()
if (NOT MYGUI_BUILD_PLUGIN_OCTREE)
  set(MYGUI_COMMENT_PLUGIN_OCTREE "#")
endif ()
if (NOT MYGUI_BUILD_PLUGIN_PCZ)
  set(MYGUI_COMMENT_PLUGIN_PCZ "#")
endif ()
if (NOT MYGUI_BUILD_PLUGIN_PFX)
  set(MYGUI_COMMENT_PLUGIN_PARTICLEFX "#")
endif ()
if (NOT MYGUI_BUILD_PLUGIN_CG)
  set(MYGUI_COMMENT_PLUGIN_CG "#")
endif ()
if (NOT MYGUI_BUILD_PLUGIN_PCZ)
  set(MYGUI_COMMENT_PLUGIN_PCZ "#")
endif ()



# CREATE CONFIG FILES - INSTALL VERSIONS
# create resources.cfg
configure_file(${MYGUI_TEMPLATES_DIR}/resources_d.cfg.in ${MYGUI_BINARY_DIR}/inst/bin/debug/resources.cfg)
configure_file(${MYGUI_TEMPLATES_DIR}/resources.cfg.in ${MYGUI_BINARY_DIR}/inst/bin/release/resources.cfg)
# create plugins.cfg
configure_file(${MYGUI_TEMPLATES_DIR}/plugins_d.cfg.in ${MYGUI_BINARY_DIR}/inst/bin/debug/plugins.cfg)
configure_file(${MYGUI_TEMPLATES_DIR}/plugins.cfg.in ${MYGUI_BINARY_DIR}/inst/bin/release/plugins.cfg)
# create media.cfg
configure_file(${MYGUI_TEMPLATES_DIR}/media.cfg.in ${MYGUI_BINARY_DIR}/inst/bin/debug/media.cfg)
configure_file(${MYGUI_TEMPLATES_DIR}/media.cfg.in ${MYGUI_BINARY_DIR}/inst/bin/release/media.cfg)
# create quake3settings.cfg
configure_file(${MYGUI_TEMPLATES_DIR}/quake3settings_d.cfg.in ${MYGUI_BINARY_DIR}/inst/bin/debug/quake3settings.cfg)
configure_file(${MYGUI_TEMPLATES_DIR}/quake3settings.cfg.in ${MYGUI_BINARY_DIR}/inst/bin/release/quake3settings.cfg)

# install resource files
if (MYGUI_INSTALL_SAMPLES OR MYGUI_INSTALL_TOOLS)
  install(FILES 
    ${MYGUI_BINARY_DIR}/inst/bin/debug/resources.cfg
    ${MYGUI_BINARY_DIR}/inst/bin/debug/plugins.cfg
    ${MYGUI_BINARY_DIR}/inst/bin/debug/media.cfg
    ${MYGUI_BINARY_DIR}/inst/bin/debug/quake3settings.cfg
    DESTINATION "bin${MYGUI_DEBUG_PATH}"
    CONFIGURATIONS Debug
  )
  install(FILES 
    ${MYGUI_BINARY_DIR}/inst/bin/release/resources.cfg
    ${MYGUI_BINARY_DIR}/inst/bin/release/plugins.cfg
    ${MYGUI_BINARY_DIR}/inst/bin/release/media.cfg
    ${MYGUI_BINARY_DIR}/inst/bin/release/quake3settings.cfg
    DESTINATION "bin${MYGUI_RELEASE_PATH}" CONFIGURATIONS Release None ""
  )
  install(FILES 
    ${MYGUI_BINARY_DIR}/inst/bin/release/resources.cfg
    ${MYGUI_BINARY_DIR}/inst/bin/release/plugins.cfg
    ${MYGUI_BINARY_DIR}/inst/bin/release/media.cfg
    ${MYGUI_BINARY_DIR}/inst/bin/release/quake3settings.cfg
	DESTINATION "bin${MYGUI_RELWDBG_PATH}" CONFIGURATIONS RelWithDebInfo
  )
  install(FILES 
    ${MYGUI_BINARY_DIR}/inst/bin/release/resources.cfg
    ${MYGUI_BINARY_DIR}/inst/bin/release/plugins.cfg
    ${MYGUI_BINARY_DIR}/inst/bin/release/media.cfg
    ${MYGUI_BINARY_DIR}/inst/bin/release/quake3settings.cfg
	DESTINATION "bin${MYGUI_MINSIZE_PATH}" CONFIGURATIONS MinSizeRel
  )
endif ()


# CREATE CONFIG FILES - BUILD DIR VERSIONS
set(MYGUI_MEDIA_DIR_REL "${MYGUI_SOURCE_DIR}/Samples/Media")
set(MYGUI_MEDIA_DIR_DBG "${MYGUI_SOURCE_DIR}/Samples/Media")
if (WIN32)
  set(MYGUI_PLUGIN_DIR_REL ".")
  set(MYGUI_PLUGIN_DIR_DBG ".")
elseif (APPLE)
  # not used on OS X, uses Resources
  set(MYGUI_PLUGIN_DIR_REL "")
  set(MYGUI_PLUGIN_DIR_DBG "")
elseif (UNIX)
  set(MYGUI_PLUGIN_DIR_REL "../lib")
  set(MYGUI_PLUGIN_DIR_DBG "../lib")
endif ()
if (WIN32)
  # create resources.cfg
  configure_file(${MYGUI_TEMPLATES_DIR}/resources_d.cfg.in ${MYGUI_BINARY_DIR}/bin/debug/resources.cfg)
  configure_file(${MYGUI_TEMPLATES_DIR}/resources.cfg.in ${MYGUI_BINARY_DIR}/bin/release/resources.cfg)
  configure_file(${MYGUI_TEMPLATES_DIR}/resources.cfg.in ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/resources.cfg)
  configure_file(${MYGUI_TEMPLATES_DIR}/resources.cfg.in ${MYGUI_BINARY_DIR}/bin/minsizerel/resources.cfg)
  # create plugins.cfg
  configure_file(${MYGUI_TEMPLATES_DIR}/plugins_d.cfg.in ${MYGUI_BINARY_DIR}/bin/debug/plugins.cfg)
  configure_file(${MYGUI_TEMPLATES_DIR}/plugins.cfg.in ${MYGUI_BINARY_DIR}/bin/release/plugins.cfg)
  configure_file(${MYGUI_TEMPLATES_DIR}/plugins.cfg.in ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/plugins.cfg)
  configure_file(${MYGUI_TEMPLATES_DIR}/plugins.cfg.in ${MYGUI_BINARY_DIR}/bin/minsizerel/plugins.cfg)
else() # other OS only need one cfg file
  string(TOLOWER "${CMAKE_BUILD_TYPE}" MYGUI_BUILD_TYPE)
  if (MYGUI_BUILD_TYPE STREQUAL "debug" AND NOT APPLE)
    set(MYGUI_CFG_SUFFIX "_d")
  endif ()
  # create resources.cfg
  configure_file(${MYGUI_TEMPLATES_DIR}/resources${MYGUI_CFG_SUFFIX}.cfg.in ${MYGUI_BINARY_DIR}/bin/resources.cfg)
  # create plugins.cfg
  configure_file(${MYGUI_TEMPLATES_DIR}/plugins${MYGUI_CFG_SUFFIX}.cfg.in ${MYGUI_BINARY_DIR}/bin/plugins.cfg)
endif ()

