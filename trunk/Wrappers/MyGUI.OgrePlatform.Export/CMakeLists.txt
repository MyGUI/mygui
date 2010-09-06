#only for ogre
if(MYGUI_RENDERSYSTEM EQUAL 2)

	function(mygui_orge_platform_export PROJECTNAME SOLUTIONFOLDER)
		include_directories(
			.
			#${MYGUI_SOURCE_DIR}/Common
			${MYGUI_SOURCE_DIR}/MyGUIEngine/include
			#${OIS_INCLUDE_DIRS}
		)
		# define the sources
		include(${PROJECTNAME}.list)
		#if(MYGUI_RENDERSYSTEM EQUAL 1)
		#	mygui_add_base_manager_source(DirectX)
		#	add_definitions("-DMYGUI_DIRECTX_PLATFORM")
		#	include_directories(
		#		${MYGUI_SOURCE_DIR}/Platforms/DirectX/DirectXPlatform/include
		#		${DirectX_INCLUDE_DIR}
		#	)
		#	link_directories(${DIRECTX_LIB_DIR})
		#elseif(MYGUI_RENDERSYSTEM EQUAL 2)
			#mygui_add_base_manager_source(Ogre)
			#add_definitions("-DMYGUI_OGRE_PLATFORM")
			include_directories(
				${MYGUI_SOURCE_DIR}/Platforms/Ogre/OgrePlatform/include
				${OGRE_INCLUDE_DIR}
			)
			link_directories(${OGRE_LIB_DIR})
		#elseif(MYGUI_RENDERSYSTEM EQUAL 3)
		#	mygui_add_base_manager_source(OpenGL)
		#	add_definitions("-DMYGUI_OPENGL_PLATFORM")
		#	include_directories(
		#		${MYGUI_SOURCE_DIR}/Platforms/OpenGL/OpenGLPlatform/include
		#		${OPENGL_INCLUDE_DIR}
		#	)
		#	link_directories(${OPENGL_LIB_DIR})
		#endif()
		
		#if(MYGUI_SAMPLES_INPUT EQUAL 1)
		#	mygui_add_input_source(OIS)
		#elseif(MYGUI_SAMPLES_INPUT EQUAL 2)
		#	mygui_add_input_source(Win32)
		#elseif(MYGUI_SAMPLES_INPUT EQUAL 3)
		#	mygui_add_input_source(Win32_OIS)
		#endif()
		
		#if (MYGUI_DONT_USE_OBSOLETE)
		#	add_definitions(-DMYGUI_DONT_USE_OBSOLETE)
		#endif ()
		
		# setup MyGUIEngine target
		#if (${SOLUTIONFOLDER} STREQUAL "Wrappers")
			add_library(${PROJECTNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
		#else ()
		#	add_executable(${PROJECTNAME} WIN32 ${HEADER_FILES} ${SOURCE_FILES})
		#endif ()
		set_target_properties(${PROJECTNAME} PROPERTIES FOLDER ${SOLUTIONFOLDER})
		
		# add dependencies
		add_dependencies(${PROJECTNAME} MyGUIEngine )

		#mygui_config_sample(${PROJECTNAME})

		#if(MYGUI_SAMPLES_INPUT EQUAL 1)
		#	add_definitions("-DMYGUI_SAMPLES_INPUT_OIS")
		#	link_directories(${OIS_LIB_DIR})
		#	target_link_libraries(${PROJECTNAME} ${OIS_LIBRARIES})
		#elseif(MYGUI_SAMPLES_INPUT EQUAL 2)
		#	add_definitions("-DMYGUI_SAMPLES_INPUT_WIN32")
		#elseif(MYGUI_SAMPLES_INPUT EQUAL 3)
		#	add_definitions("-DMYGUI_SAMPLES_INPUT_WIN32_OIS")
		#	link_directories(${OIS_LIB_DIR})
		#	target_link_libraries(${PROJECTNAME} ${OIS_LIBRARIES})
		#endif()
		
		# link libraries against it
		target_link_libraries(${PROJECTNAME}
			MyGUIEngine
		#	uuid
		)

		# add dependencies
		add_dependencies(${PROJECTNAME} MyGUIEngine)
		#if(MYGUI_RENDERSYSTEM EQUAL 1)
		#	add_dependencies(${PROJECTNAME} MyGUI.DirectXPlatform)
		#	target_link_libraries(${PROJECTNAME} MyGUI.DirectXPlatform)
		#elseif(MYGUI_RENDERSYSTEM EQUAL 2)
			add_dependencies(${PROJECTNAME} MyGUI.OgrePlatform)
			target_link_libraries(${PROJECTNAME} MyGUI.OgrePlatform)
		#elseif(MYGUI_RENDERSYSTEM EQUAL 3)
		#	add_dependencies(${PROJECTNAME} MyGUI.OpenGLPlatform)
		#	target_link_libraries(${PROJECTNAME} MyGUI.OpenGLPlatform)
		#endif()
	endfunction(mygui_orge_platform_export)

	mygui_orge_platform_export(MyGUI.OgrePlatform.Export Wrappers)

endif()
