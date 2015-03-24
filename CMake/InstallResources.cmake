##################################################################
# Generate and install the config files needed for the samples
##################################################################


function(install_file FILENAME)
	if (WIN32)
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
			${MYGUI_BINARY_DIR}/bin/release/${FILENAME}
			DESTINATION "bin${MYGUI_MINSIZE_PATH}" CONFIGURATIONS MinSizeRel
		)
	else ()
		install(FILES
			${MYGUI_BINARY_DIR}/bin/${FILENAME}
			DESTINATION "bin"
		)
	endif ()
endfunction(install_file)

if (MYGUI_RENDERSYSTEM EQUAL 3)
	if (WIN32)
		option(MYGUI_OGRE_WAS_BUILT_WITH_DIRECTX "Ogre have DirectX render ssystem and we don't need to disable it" TRUE)
		MARK_AS_ADVANCED(MYGUI_OGRE_WAS_BUILT_WITH_DIRECTX)
		
		set(MYGUI_PLUGIN_DIR_DBG ".")
		set(MYGUI_PLUGIN_DIR_REL ".")
		if (MYGUI_OGRE_WAS_BUILT_WITH_DIRECTX)
			set(MYGUI_NO_PLUGIN_OGRE_DIRECTX "")
		else ()
			set(MYGUI_NO_PLUGIN_OGRE_DIRECTX "#")
		endif ()
	elseif (UNIX)
		set(MYGUI_PLUGIN_DIR_DBG ${OGRE_PLUGIN_DIR_DBG})
		set(MYGUI_PLUGIN_DIR_REL ${OGRE_PLUGIN_DIR_REL})
		set(MYGUI_NO_PLUGIN_OGRE_DIRECTX "#")
	endif()
endif ()

# install resource files
if (MYGUI_INSTALL_SAMPLES OR MYGUI_INSTALL_TOOLS)
	if (WIN32)
		set(MYGUI_MEDIA_DIR "../../Media")
	elseif (UNIX)
		set(MYGUI_MEDIA_DIR "../share/MYGUI/Media")
	else ()
		set(MYGUI_MEDIA_DIR "../../Media")
	endif ()
	if (WIN32)
		# create resources.xml
		configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/debug/resources.xml)
		configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/release/resources.xml)
		if (MYGUI_RENDERSYSTEM EQUAL 3)
			# create plugins.cfg
			configure_file(${MYGUI_TEMPLATES_DIR}/plugins_d.cfg.in ${MYGUI_BINARY_DIR}/bin/debug/plugins.cfg)
			configure_file(${MYGUI_TEMPLATES_DIR}/plugins.cfg.in ${MYGUI_BINARY_DIR}/bin/release/plugins.cfg)
		endif ()
	else() # other OS only need one cfg file
		string(TOLOWER "${CMAKE_BUILD_TYPE}" MYGUI_BUILD_TYPE)
		if (MYGUI_BUILD_TYPE STREQUAL "debug" AND NOT APPLE)
			set(MYGUI_CFG_SUFFIX "_d")
		endif ()
		# create resources.xml
		configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/resources.xml)
		if (MYGUI_RENDERSYSTEM EQUAL 3)
			# create plugins.cfg
			configure_file(${MYGUI_TEMPLATES_DIR}/plugins${MYGUI_CFG_SUFFIX}.cfg.in ${MYGUI_BINARY_DIR}/bin/plugins.cfg)
		endif ()
	endif ()
	
	install_file (resources.xml)
	if (MYGUI_RENDERSYSTEM EQUAL 3)
		install_file (plugins.cfg)
	endif ()
else ()

	set(MYGUI_MEDIA_DIR "${MYGUI_SOURCE_DIR}/Media")

	if (WIN32)
		# create resources.xml
		configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/debug/resources.xml)
		configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/release/resources.xml)
		configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/resources.xml)
		configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/minsizerel/resources.xml)
		if (MYGUI_RENDERSYSTEM EQUAL 3)
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
		if (MYGUI_RENDERSYSTEM EQUAL 3)
			# create plugins.cfg
			configure_file(${MYGUI_TEMPLATES_DIR}/plugins${MYGUI_CFG_SUFFIX}.cfg.in ${MYGUI_BINARY_DIR}/bin/plugins.cfg)
		endif ()
	endif ()

endif ()
