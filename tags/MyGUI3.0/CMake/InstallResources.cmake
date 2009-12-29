##################################################################
# Generate and install the config files needed for the samples
##################################################################


#setup Plugin builds
function(install_file FILENAME)
	install(FILES
		${MYGUI_BINARY_DIR}/bin/debug/${FILENAME}
		DESTINATION "bin${MYGUI_DEBUG_PATH}" CONFIGURATIONS Debug
	)
	install(FILES
		${MYGUI_BINARY_DIR}/bin/release/${FILENAME}
		DESTINATION "bin${MYGUI_RELEASE_PATH}" CONFIGURATIONS Release None ""
	)
	install(FILES
		${MYGUI_BINARY_DIR}/bin/release/${FILENAME}
		DESTINATION "bin${MYGUI_RELWDBG_PATH}" CONFIGURATIONS RelWithDebInfo
	)
	install(FILES
		${MYGUI_BINARY_DIR}/bin/release/resources.xml
		${MYGUI_BINARY_DIR}/bin/release/${FILENAME}
		DESTINATION "bin${MYGUI_MINSIZE_PATH}" CONFIGURATIONS MinSizeRel
	)
endfunction(install_file)

if (WIN32)
  set(MYGUI_MEDIA_PATH "media")
  set(MYGUI_MEDIA_DIR_REL "../../${MYGUI_MEDIA_PATH}")
  set(MYGUI_MEDIA_DIR_DBG "../../${MYGUI_MEDIA_PATH}")
  if (MYGUI_RENDERSYSTEM EQUAL 2)
    set(MYGUI_PLUGIN_DIR ".")
    set(MYGUI_PLUGIN_OGRE_DIRECTX "Plugin=RenderSystem_Direct3D9")
  endif()
elseif (UNIX)
  set(MYGUI_MEDIA_PATH "share/MYGUI/media")
  set(MYGUI_MEDIA_DIR_REL "../${MYGUI_MEDIA_PATH}")
  set(MYGUI_MEDIA_DIR_DBG "../../${MYGUI_MEDIA_PATH}")
  if (MYGUI_RENDERSYSTEM EQUAL 2)
    set(MYGUI_PLUGIN_DIR "/usr/lib/OGRE")
    set(MYGUI_PLUGIN_OGRE_DIRECTX "")
  endif()
endif ()

# create resources.xml
configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/debug/resources.xml)
configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/release/resources.xml)
if (MYGUI_RENDERSYSTEM EQUAL 2)
	# create plugins.cfg
	configure_file(${MYGUI_TEMPLATES_DIR}/plugins_d.cfg.in ${MYGUI_BINARY_DIR}/bin/debug/plugins.cfg)
	configure_file(${MYGUI_TEMPLATES_DIR}/plugins.cfg.in ${MYGUI_BINARY_DIR}/bin/release/plugins.cfg)
endif ()

# install resource files
if (MYGUI_INSTALL_SAMPLES OR MYGUI_INSTALL_TOOLS)
	install_file (resources.xml)
	if (MYGUI_RENDERSYSTEM EQUAL 2)
		install_file (plugins.cfg)
	endif ()
endif ()

if (WIN32)
	# create resources.xml
	configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/debug/resources.xml)
	configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/release/resources.xml)
	configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/resources.xml)
	configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/minsizerel/resources.xml)
	if (MYGUI_RENDERSYSTEM EQUAL 2)
		# create plugins.cfg
		configure_file(${MYGUI_TEMPLATES_DIR}/plugins_d.cfg.in ${MYGUI_BINARY_DIR}/bin/debug/plugins.cfg)
		configure_file(${MYGUI_TEMPLATES_DIR}/plugins.cfg.in ${MYGUI_BINARY_DIR}/bin/release/plugins.cfg)
		configure_file(${MYGUI_TEMPLATES_DIR}/plugins.cfg.in ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/plugins.cfg)
		configure_file(${MYGUI_TEMPLATES_DIR}/plugins.cfg.in ${MYGUI_BINARY_DIR}/bin/minsizerel/plugins.cfg)
	endif ()
else() # other OS only need one cfg file
	string(TOLOWER "${CMAKE_BUILD_TYPE}" MYGUI_BUILD_TYPE)
	if (MYGUI_BUILD_TYPE STREQUAL "debug" AND NOT APPLE)
		set(MYGUI_CFG_SUFFIX "_d")
	endif ()
	# create resources.xml
	configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/resources.xml)
	if (MYGUI_RENDERSYSTEM EQUAL 2)
		# create plugins.cfg
		configure_file(${MYGUI_TEMPLATES_DIR}/plugins${MYGUI_CFG_SUFFIX}.cfg.in ${MYGUI_BINARY_DIR}/bin/plugins.cfg)
	endif ()
endif ()

