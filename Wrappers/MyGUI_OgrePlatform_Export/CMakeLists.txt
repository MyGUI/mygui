#only for ogre
if(MYGUI_RENDERSYSTEM EQUAL 3)

	function(mygui_orge_platform_export PROJECTNAME SOLUTIONFOLDER)
		include_directories(
			.
			${MYGUI_SOURCE_DIR}/MyGUIEngine/include
		)
		# define the sources
		include(${PROJECTNAME}.list)
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/Ogre/OgrePlatform/include
			${OGRE_INCLUDE_DIR}
		)
		link_directories(${OGRE_LIB_DIR})
		add_library(${PROJECTNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
		set_target_properties(${PROJECTNAME} PROPERTIES FOLDER ${SOLUTIONFOLDER})

		target_link_libraries(${PROJECTNAME} MyGUIEngine MyGUI.OgrePlatform)
	endfunction(mygui_orge_platform_export)

	mygui_orge_platform_export(MyGUI_OgrePlatform_Export Wrappers)

endif()
