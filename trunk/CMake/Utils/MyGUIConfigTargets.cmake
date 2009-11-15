# Configure settings and install targets
# FIXME: Copypasted from Ogre and need lots of changes

if (WIN32)
  set(MYGUI_RELEASE_PATH "/Release")
  set(MYGUI_RELWDBG_PATH "/RelWithDebInfo")
  set(MYGUI_MINSIZE_PATH "/MinSizeRel")
  set(MYGUI_DEBUG_PATH "/Debug")
  set(MYGUI_LIB_RELEASE_PATH "/Release")
  set(MYGUI_LIB_RELWDBG_PATH "/RelWithDebInfo")
  set(MYGUI_LIB_MINSIZE_PATH "/MinSizeRel")
  set(MYGUI_LIB_DEBUG_PATH "/Debug")
  set(MYGUI_PLUGIN_PATH "/opt")
elseif (UNIX)
  set(MYGUI_RELEASE_PATH "")
  set(MYGUI_RELWDBG_PATH "")
  set(MYGUI_MINSIZE_PATH "")
  set(MYGUI_DEBUG_PATH "/debug")
  set(MYGUI_LIB_RELEASE_PATH "")
  set(MYGUI_LIB_RELWDBG_PATH "")
  set(MYGUI_LIB_MINSIZE_PATH "")
  set(MYGUI_LIB_DEBUG_PATH "")
  set(MYGUI_PLUGIN_PATH "/MYGUI")
endif ()

# create vcproj.user file for Visual Studio to set debug working directory
function(mygui_create_vcproj_userfile TARGETNAME)
  if (MSVC)
    configure_file(
	  ${MYGUI_TEMPLATES_DIR}/VisualStudioUserFile.vcproj.user.in
	  ${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}.vcproj.user
	  @ONLY
	)
  endif ()
endfunction(mygui_create_vcproj_userfile)

# install targets according to current build type
function(mygui_install_target TARGETNAME SUFFIX)
  install(TARGETS ${TARGETNAME}
    RUNTIME DESTINATION "bin${MYGUI_RELEASE_PATH}" CONFIGURATIONS Release None ""
    LIBRARY DESTINATION "lib${MYGUI_LIB_RELEASE_PATH}${SUFFIX}" CONFIGURATIONS Release None ""
    ARCHIVE DESTINATION "lib${MYGUI_LIB_RELEASE_PATH}${SUFFIX}" CONFIGURATIONS Release None ""
    FRAMEWORK DESTINATION "bin${MYGUI_RELEASE_PATH}" CONFIGURATIONS Release None ""
  )
  install(TARGETS ${TARGETNAME}
    RUNTIME DESTINATION "bin${MYGUI_RELWDBG_PATH}" CONFIGURATIONS RelWithDebInfo
    LIBRARY DESTINATION "lib${MYGUI_LIB_RELWDBG_PATH}${SUFFIX}" CONFIGURATIONS RelWithDebInfo
    ARCHIVE DESTINATION "lib${MYGUI_LIB_RELWDBG_PATH}${SUFFIX}" CONFIGURATIONS RelWithDebInfo
    FRAMEWORK DESTINATION "bin${MYGUI_RELWDBG_PATH}" CONFIGURATIONS RelWithDebInfo
  )
  install(TARGETS ${TARGETNAME}
    RUNTIME DESTINATION "bin${MYGUI_MINSIZE_PATH}" CONFIGURATIONS MinSizeRel
    LIBRARY DESTINATION "lib${MYGUI_LIB_MINSIZE_PATH}${SUFFIX}" CONFIGURATIONS MinSizeRel
    ARCHIVE DESTINATION "lib${MYGUI_LIB_MINSIZE_PATH}${SUFFIX}" CONFIGURATIONS MinSizeRel
    FRAMEWORK DESTINATION "bin${MYGUI_MINSIZE_PATH}" CONFIGURATIONS MinSizeRel
  )
  install(TARGETS ${TARGETNAME}
    RUNTIME DESTINATION "bin${MYGUI_DEBUG_PATH}" CONFIGURATIONS Debug
    LIBRARY DESTINATION "lib${MYGUI_LIB_DEBUG_PATH}${SUFFIX}" CONFIGURATIONS Debug
    ARCHIVE DESTINATION "lib${MYGUI_LIB_DEBUG_PATH}${SUFFIX}" CONFIGURATIONS Debug
    FRAMEWORK DESTINATION "bin${MYGUI_DEBUG_PATH}" CONFIGURATIONS Debug
  )
endfunction(mygui_install_target)

# setup common target settings
function(mygui_config_common TARGETNAME)
  set_target_properties(${TARGETNAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${MYGUI_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${MYGUI_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${MYGUI_BINARY_DIR}/bin
  )
  mygui_create_vcproj_userfile(${TARGETNAME})
endfunction(mygui_config_common)

#setup Demo builds
function(mygui_demo DEMONAME)
	include_directories(
		.
		${MYGUI_SOURCE_DIR}/Common
		${MYGUI_SOURCE_DIR}/MyGUIEngine/include
		${OIS_INCLUDE_DIRS}
	)
	# define the sources
	include(${DEMONAME}.list)
	if(MYGUI_RENDERSYSTEM EQUAL 1)
		add_definitions("-DMYGUI_DIRECTX_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/DirectX/DirectXPlatform/include
			${DIRECTX_INCLUDE_DIR}
		)
		link_directories(${DIRECTX_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 2)
		add_definitions("-DMYGUI_OGRE_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/Ogre/OgrePlatform/include
			${OGRE_INCLUDE_DIR}
		)
		link_directories(${OGRE_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 3)
		add_definitions("-DMYGUI_OPENGL_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/OpenGL/OpenGLPlatform/include
			${OPENGL_INCLUDE_DIR}
		)
		link_directories(${OPENGL_LIB_DIR})
	endif()
	
	# setup MyGUIEngine target
	add_executable(${DEMONAME} WIN32 ${HEADER_FILES} ${SOURCE_FILES})
	set_target_properties(${DEMONAME} PROPERTIES SOLUTION_FOLDER "Demos")
	
	# add dependencies
	add_dependencies(${DEMONAME} MyGUIEngine )

	mygui_config_sample(${DEMONAME})

	if(MYGUI_SAMPLES_INPUT EQUAL 1)
		add_definitions("-DMYGUI_SAMPLES_INPUT_OIS")
		link_directories(${OIS_LIB_DIR})
		target_link_libraries(${DEMONAME} ${OIS_LIBRARIES})
	elseif(MYGUI_SAMPLES_INPUT EQUAL 2)
		add_definitions("-DMYGUI_SAMPLES_INPUT_WIN32")
	endif()
	
	# link libraries against it
	target_link_libraries(${DEMONAME}
		MyGUIEngine
		uuid
	)

	add_definitions("-DMYGUI_SOURCE_DIR=\"${MYGUI_SOURCE_DIR}\"")
	
	# add dependencies
	add_dependencies(${DEMONAME} MyGUIEngine)
	if(MYGUI_RENDERSYSTEM EQUAL 1)
		add_dependencies(${DEMONAME} MyGUI.DirectXPlatform)
		target_link_libraries(${DEMONAME} MyGUI.DirectXPlatform)
	elseif(MYGUI_RENDERSYSTEM EQUAL 2)
		add_dependencies(${DEMONAME} MyGUI.OgrePlatform)
		target_link_libraries(${DEMONAME} MyGUI.OgrePlatform)
	elseif(MYGUI_RENDERSYSTEM EQUAL 3)
		add_dependencies(${DEMONAME} MyGUI.OpenGLPlatform)
		target_link_libraries(${DEMONAME} MyGUI.OpenGLPlatform)
	endif()
	
	# install debug pdb files
	install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_DEBUG_PATH}/${DEMONAME}.pdb
		DESTINATION bin${MYGUI_DEBUG_PATH} CONFIGURATIONS Debug
	)
	install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_RELWDBG_PATH}/${DEMONAME}.pdb
		DESTINATION bin${MYGUI_RELWDBG_PATH} CONFIGURATIONS RelWithDebInfo
	)

	mygui_install_target(${DEMONAME} "")
endfunction(mygui_demo)

#setup Plugin builds
function(mygui_plugin PLUGINNAME)
	include_directories(.)

	# define the sources
	include(${PLUGINNAME}.list)
	
	add_definitions("-D_USRDLL -DMYGUI_BUILD_DLL")
	
	# setup MyGUIEngine target
	add_library(${PLUGINNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
	
	# add dependencies
	add_dependencies(${PLUGINNAME} MyGUIEngine)

	mygui_config_common(${PLUGINNAME})

	# link libraries against it
	target_link_libraries(${PLUGINNAME}
		MyGUIEngine
	)
	
	mygui_install_target(${PLUGINNAME} "")
	
	install(FILES ${HEADER_FILES}
		DESTINATION include/MyGUIPlugins/${PLUGINNAME}
	)

endfunction(mygui_plugin)



# setup library build
function(mygui_config_lib LIBNAME)
  mygui_config_common(${LIBNAME})
  if (MYGUI_STATIC)
    # add static prefix, if compiling static version
    set_target_properties(${LIBNAME} PROPERTIES OUTPUT_NAME ${LIBNAME}Static)
  else (MYGUI_STATIC)
    if (CMAKE_COMPILER_IS_GNUCXX)
      # add GCC visibility flags to shared library build
      set_target_properties(${LIBNAME} PROPERTIES COMPILE_FLAGS "${MYGUI_GCC_VISIBILITY_FLAGS}")
	endif (CMAKE_COMPILER_IS_GNUCXX)
  endif (MYGUI_STATIC)
  mygui_install_target(${LIBNAME} "")
  
  if (MYGUI_INSTALL_PDB)
    # install debug pdb files
    if (MYGUI_STATIC)
	  install(FILES ${MYGUI_BINARY_DIR}/lib${MYGUI_LIB_DEBUG_PATH}/${LIBNAME}Static_d.pdb
	    DESTINATION lib${MYGUI_LIB_DEBUG_PATH}
		CONFIGURATIONS Debug
	  )
	  install(FILES ${MYGUI_BINARY_DIR}/lib${MYGUI_LIB_RELWDBG_PATH}/${LIBNAME}Static.pdb
	    DESTINATION lib${MYGUI_LIB_RELWDBG_PATH}
		CONFIGURATIONS RelWithDebInfo
	  )
	else ()
	  install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_DEBUG_PATH}/${LIBNAME}_d.pdb
	    DESTINATION bin${MYGUI_DEBUG_PATH}
		CONFIGURATIONS Debug
	  )
	  install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_RELWDBG_PATH}/${LIBNAME}.pdb
	    DESTINATION bin${MYGUI_RELWDBG_PATH}
		CONFIGURATIONS RelWithDebInfo
	  )
	endif ()
  endif ()
endfunction(mygui_config_lib)

# setup plugin build
function(mygui_config_plugin PLUGINNAME)
  mygui_config_common(${PLUGINNAME})
  if (MYGUI_STATIC)
    # add static prefix, if compiling static version
    set_target_properties(${PLUGINNAME} PROPERTIES OUTPUT_NAME ${PLUGINNAME}Static)
  else (MYGUI_STATIC)
    if (CMAKE_COMPILER_IS_GNUCXX)
      # add GCC visibility flags to shared library build
      set_target_properties(${PLUGINNAME} PROPERTIES COMPILE_FLAGS "${MYGUI_GCC_VISIBILITY_FLAGS}")
      # disable "lib" prefix on Unix
      set_target_properties(${PLUGINNAME} PROPERTIES PREFIX "")
	endif (CMAKE_COMPILER_IS_GNUCXX)	
  endif (MYGUI_STATIC)
  mygui_install_target(${PLUGINNAME} ${MYGUI_PLUGIN_PATH})

  if (MYGUI_INSTALL_PDB)
    # install debug pdb files
    if (MYGUI_STATIC)
	  install(FILES ${MYGUI_BINARY_DIR}/lib${MYGUI_LIB_DEBUG_PATH}/${PLUGINNAME}Static_d.pdb
	    DESTINATION lib${MYGUI_LIB_DEBUG_PATH}/opt
		CONFIGURATIONS Debug
	  )
	  install(FILES ${MYGUI_BINARY_DIR}/lib${MYGUI_LIB_RELWDBG_PATH}/${PLUGINNAME}Static.pdb
	    DESTINATION lib${MYGUI_LIB_RELWDBG_PATH}/opt
		CONFIGURATIONS RelWithDebInfo
	  )
	else ()
	  install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_DEBUG_PATH}/${PLUGINNAME}_d.pdb
	    DESTINATION bin${MYGUI_DEBUG_PATH}
		CONFIGURATIONS Debug
	  )
	  install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_RELWDBG_PATH}/${PLUGINNAME}.pdb
	    DESTINATION bin${MYGUI_RELWDBG_PATH}
		CONFIGURATIONS RelWithDebInfo
	  )
	endif ()
  endif ()
endfunction(mygui_config_plugin)

# setup Ogre demo build
function(mygui_config_sample SAMPLENAME)
  mygui_config_common(${SAMPLENAME})

  # set install RPATH for Unix systems
  if (UNIX AND MYGUI_FULL_RPATH)
    set_property(TARGET ${SAMPLENAME} APPEND PROPERTY
      INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
    set_property(TARGET ${SAMPLENAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
  endif ()
  
endfunction(mygui_config_sample)

# setup Ogre tool build
function(mygui_config_tool TOOLNAME)
  mygui_config_common(${TOOLNAME})

  # set install RPATH for Unix systems
  if (UNIX AND MYGUI_FULL_RPATH)
    set_property(TARGET ${TOOLNAME} APPEND PROPERTY
      INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
    set_property(TARGET ${TOOLNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
  endif ()

  if (MYGUI_INSTALL_TOOLS)
    mygui_install_target(${TOOLNAME} "")
    if (MYGUI_INSTALL_PDB)
      # install debug pdb files
      install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_DEBUG_PATH}/${TOOLNAME}.pdb
        DESTINATION bin${MYGUI_DEBUG_PATH}
        CONFIGURATIONS Debug
        )
      install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_RELWDBG_PATH}/${TOOLNAME}.pdb
        DESTINATION bin${MYGUI_RELWDBG_PATH}
        CONFIGURATIONS RelWithDebInfo
        )
    endif ()
  endif ()	

endfunction(mygui_config_tool)
