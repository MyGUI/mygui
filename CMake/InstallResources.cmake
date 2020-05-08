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


# install resource files
if (MYGUI_INSTALL_SAMPLES OR MYGUI_INSTALL_TOOLS)
	if (MYGUI_RENDERSYSTEM EQUAL 3)
		# copy plugins.cfg
		if (DEFINED OGRE_CONFIG_DIR)
			if (WIN32)
				if(EXISTS ${OGRE_CONFIG_DIR}/plugins_d.cfg)
					configure_file(${OGRE_CONFIG_DIR}/plugins_d.cfg ${MYGUI_BINARY_DIR}/bin/debug/plugins.cfg)
				else()
					file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg DESTINATION ${MYGUI_BINARY_DIR}/bin/debug/)
				endif ()
				file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg DESTINATION ${MYGUI_BINARY_DIR}/bin/release/)
				file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg DESTINATION ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/)
				file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg DESTINATION ${MYGUI_BINARY_DIR}/bin/minsizerel/)
			else()
				file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg DESTINATION ${MYGUI_BINARY_DIR}/bin/)
			endif()
		endif()
	endif ()

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
	else() # other OS only need one cfg file
		# create resources.xml
		configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/resources.xml)
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
	else() # other OS only need one cfg file
		string(TOLOWER "${CMAKE_BUILD_TYPE}" MYGUI_BUILD_TYPE)
		if (MYGUI_BUILD_TYPE STREQUAL "debug" AND NOT APPLE)
			set(MYGUI_CFG_SUFFIX "_d")
		endif ()
		# create resources.xml
		configure_file(${MYGUI_TEMPLATES_DIR}/resources.xml.in ${MYGUI_BINARY_DIR}/bin/resources.xml)
	endif ()

endif ()
