##################################################################
# Generate and install the config files needed for the samples
##################################################################


function(install_file FILENAME)
	install(FILES
		"${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/${FILENAME}"
		DESTINATION "${CMAKE_INSTALL_BINDIR}"
	)
endfunction(install_file)


# install resource files
if (MYGUI_INSTALL_DEMOS OR MYGUI_INSTALL_TOOLS)
	if (MYGUI_RENDERSYSTEM EQUAL 3)
		# copy plugins.cfg
		if (DEFINED OGRE_CONFIG_DIR)
			file(COPY "${OGRE_CONFIG_DIR}/plugins.cfg" DESTINATION "${MYGUI_BINARY_DIR}/bin/")
			install_file (plugins.cfg)
		endif()
	endif ()

	if (UNIX)
		set(MYGUI_MEDIA_DIR "../share/MYGUI/Media")
	else ()
		set(MYGUI_MEDIA_DIR "../../Media")
	endif ()

	# create resources.xml
	configure_file("${MYGUI_TEMPLATES_DIR}/resources.xml.in" "${MYGUI_BINARY_DIR}/bin/resources.xml")
	install_file (resources.xml)
else ()
	set(MYGUI_MEDIA_DIR "${MYGUI_SOURCE_DIR}/Media")
	if (EMSCRIPTEN)
		set(MYGUI_MEDIA_DIR "/Media")
	endif ()

	# create resources.xml
	configure_file("${MYGUI_TEMPLATES_DIR}/resources.xml.in" "${MYGUI_BINARY_DIR}/bin/resources.xml")
endif ()
