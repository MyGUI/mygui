#only for ogre
if(MYGUI_RENDERSYSTEM EQUAL 3)

	function(mygui_orge_platform_export PROJECTNAME SOLUTIONFOLDER)
		# define the sources
		include(${PROJECTNAME}.list)
		add_library(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})
		target_include_directories(${PROJECTNAME} PRIVATE "${OGRE_INCLUDE_DIR}")
		target_link_directories(${PROJECTNAME} PUBLIC "${OGRE_LIB_DIR}")
		set_target_properties(${PROJECTNAME} PROPERTIES FOLDER ${SOLUTIONFOLDER})

		target_link_libraries(${PROJECTNAME} PUBLIC MyGUI::MyGUI MyGUI::OgrePlatform)
	endfunction(mygui_orge_platform_export)

	mygui_orge_platform_export(MyGUI_OgrePlatform_Export Wrappers)

endif()
