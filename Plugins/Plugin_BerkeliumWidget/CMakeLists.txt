function(mygui_plugin_berkelium PROJECTNAME)
	include_directories(.)
	
	# define the sources
	include(${PROJECTNAME}.list)
	
	add_definitions("-D_USRDLL -DMYGUI_BUILD_DLL")
	add_library(${PROJECTNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
	set_target_properties(${PROJECTNAME} PROPERTIES SOLUTION_FOLDER "Plugins")
	add_dependencies(${PROJECTNAME} MyGUIEngine)
	target_link_libraries(${PROJECTNAME} MyGUIEngine)
	
	mygui_config_lib(${PROJECTNAME})
	
	install(FILES ${HEADER_FILES}
		DESTINATION include/MYGUI
	)
endfunction(mygui_plugin_berkelium)


mygui_plugin_berkelium(Plugin_BerkeliumWidget)
