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
function(mygui_demo PROJECTNAME)
	include_directories(
		.
		${MYGUI_SOURCE_DIR}/Common
		${MYGUI_SOURCE_DIR}/MyGUIEngine/include
		${OIS_INCLUDE_DIRS}
	)
	# define the sources
	include(${PROJECTNAME}.list)
	if(MYGUI_RENDERSYSTEM EQUAL 1)
		add_definitions("-DMYGUI_DIRECTX_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/DirectX/DirectXPlatform/include
			${DirectX_INCLUDE_DIR}
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
	
	if (MYGUI_DONT_USE_OBSOLETE)
		add_definitions(-DMYGUI_DONT_USE_OBSOLETE)
	endif ()
	
	# setup MyGUIEngine target
	add_executable(${PROJECTNAME} WIN32 ${HEADER_FILES} ${SOURCE_FILES})
	set_target_properties(${PROJECTNAME} PROPERTIES SOLUTION_FOLDER "Demos")
	
	# add dependencies
	add_dependencies(${PROJECTNAME} MyGUIEngine )

	mygui_config_sample(${PROJECTNAME})

	if(MYGUI_SAMPLES_INPUT EQUAL 1)
		add_definitions("-DMYGUI_SAMPLES_INPUT_OIS")
		link_directories(${OIS_LIB_DIR})
		target_link_libraries(${PROJECTNAME} ${OIS_LIBRARIES})
	elseif(MYGUI_SAMPLES_INPUT EQUAL 2)
		add_definitions("-DMYGUI_SAMPLES_INPUT_WIN32")
	elseif(MYGUI_SAMPLES_INPUT EQUAL 3)
		add_definitions("-DMYGUI_SAMPLES_INPUT_WIN32_OIS")
		link_directories(${OIS_LIB_DIR})
		target_link_libraries(${PROJECTNAME} ${OIS_LIBRARIES})
	endif()
	
	# link libraries against it
	target_link_libraries(${PROJECTNAME}
		MyGUIEngine
		uuid
	)

	# add dependencies
	add_dependencies(${PROJECTNAME} MyGUIEngine)
	if(MYGUI_RENDERSYSTEM EQUAL 1)
		add_dependencies(${PROJECTNAME} MyGUI.DirectXPlatform)
		target_link_libraries(${PROJECTNAME} MyGUI.DirectXPlatform)
	elseif(MYGUI_RENDERSYSTEM EQUAL 2)
		add_dependencies(${PROJECTNAME} MyGUI.OgrePlatform)
		target_link_libraries(${PROJECTNAME} MyGUI.OgrePlatform)
	elseif(MYGUI_RENDERSYSTEM EQUAL 3)
		add_dependencies(${PROJECTNAME} MyGUI.OpenGLPlatform)
		target_link_libraries(${PROJECTNAME} MyGUI.OpenGLPlatform)
	endif()
	
	# install debug pdb files
	install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_DEBUG_PATH}/${PROJECTNAME}.pdb
		DESTINATION bin${MYGUI_DEBUG_PATH} CONFIGURATIONS Debug
	)
	install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_RELWDBG_PATH}/${PROJECTNAME}.pdb
		DESTINATION bin${MYGUI_RELWDBG_PATH} CONFIGURATIONS RelWithDebInfo
	)

	mygui_install_target(${PROJECTNAME} "")
endfunction(mygui_demo)

function(mygui_wrapper PROJECTNAME)
	include_directories(
		.
	)
	# define the sources
	include(${PROJECTNAME}.list)
	
	add_definitions("-DMYGUI_SOURCE_DIR=\"${MYGUI_SOURCE_DIR}\"")

	# setup MyGUIEngine target
	add_executable(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})
	set_target_properties(${PROJECTNAME} PROPERTIES SOLUTION_FOLDER "Wrapper")
	
	# add dependencies
	#add_dependencies(${PROJECTNAME} MyGUIEngine )

	mygui_config_sample(${PROJECTNAME})

	# install debug pdb files
	install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_DEBUG_PATH}/${PROJECTNAME}.pdb
		DESTINATION bin${MYGUI_DEBUG_PATH} CONFIGURATIONS Debug
	)
	install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_RELWDBG_PATH}/${PROJECTNAME}.pdb
		DESTINATION bin${MYGUI_RELWDBG_PATH} CONFIGURATIONS RelWithDebInfo
	)

	mygui_install_target(${PROJECTNAME} "")
endfunction(mygui_wrapper)


#setup Wrapper base app builds
function(mygui_wrapper_base_app PROJECTNAME)
	include_directories(
		.
		${MYGUI_SOURCE_DIR}/Common
		${MYGUI_SOURCE_DIR}/MyGUIEngine/include
		${OIS_INCLUDE_DIRS}
	)
	# define the sources
	include(${PROJECTNAME}.list)
	if(MYGUI_RENDERSYSTEM EQUAL 1)
		add_definitions("-DMYGUI_DIRECTX_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/DirectX/DirectXPlatform/include
			${DirectX_INCLUDE_DIR}
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
	
	if (MYGUI_DONT_USE_OBSOLETE)
		add_definitions(-DMYGUI_DONT_USE_OBSOLETE)
	endif ()
	
	# setup MyGUIEngine target
	add_library(${PROJECTNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
	set_target_properties(${PROJECTNAME} PROPERTIES SOLUTION_FOLDER "Wrapper")
	
	# add dependencies
	add_dependencies(${PROJECTNAME} MyGUIEngine )

	mygui_config_sample(${PROJECTNAME})

	if(MYGUI_SAMPLES_INPUT EQUAL 1)
		add_definitions("-DMYGUI_SAMPLES_INPUT_OIS")
		link_directories(${OIS_LIB_DIR})
		target_link_libraries(${PROJECTNAME} ${OIS_LIBRARIES})
	elseif(MYGUI_SAMPLES_INPUT EQUAL 2)
		add_definitions("-DMYGUI_SAMPLES_INPUT_WIN32")
	elseif(MYGUI_SAMPLES_INPUT EQUAL 3)
		add_definitions("-DMYGUI_SAMPLES_INPUT_WIN32_OIS")
		link_directories(${OIS_LIB_DIR})
		target_link_libraries(${PROJECTNAME} ${OIS_LIBRARIES})
	endif()
	
	# link libraries against it
	target_link_libraries(${PROJECTNAME}
		MyGUIEngine
		uuid
	)

	# add dependencies
	add_dependencies(${PROJECTNAME} MyGUIEngine)
	if(MYGUI_RENDERSYSTEM EQUAL 1)
		add_dependencies(${PROJECTNAME} MyGUI.DirectXPlatform)
		target_link_libraries(${PROJECTNAME} MyGUI.DirectXPlatform)
	elseif(MYGUI_RENDERSYSTEM EQUAL 2)
		add_dependencies(${PROJECTNAME} MyGUI.OgrePlatform)
		target_link_libraries(${PROJECTNAME} MyGUI.OgrePlatform)
	elseif(MYGUI_RENDERSYSTEM EQUAL 3)
		add_dependencies(${PROJECTNAME} MyGUI.OpenGLPlatform)
		target_link_libraries(${PROJECTNAME} MyGUI.OpenGLPlatform)
	endif()
	
	# install debug pdb files
	install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_DEBUG_PATH}/${PROJECTNAME}.pdb
		DESTINATION bin${MYGUI_DEBUG_PATH} CONFIGURATIONS Debug
	)
	install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_RELWDBG_PATH}/${PROJECTNAME}.pdb
		DESTINATION bin${MYGUI_RELWDBG_PATH} CONFIGURATIONS RelWithDebInfo
	)

	mygui_install_target(${PROJECTNAME} "")
endfunction(mygui_wrapper_base_app)


#setup Plugin builds
function(mygui_plugin PROJECTNAME)
	include_directories(.)

	# define the sources
	include(${PROJECTNAME}.list)
	
	add_definitions("-D_USRDLL -DMYGUI_BUILD_DLL")
	
	# setup MyGUIEngine target
	add_library(${PROJECTNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
	
	# add dependencies
	add_dependencies(${PROJECTNAME} MyGUIEngine)

	mygui_config_common(${PROJECTNAME})

	# link libraries against it
	target_link_libraries(${PROJECTNAME}
		MyGUIEngine
	)
	
	mygui_install_target(${PROJECTNAME} "")
	
	install(FILES ${HEADER_FILES}
		DESTINATION include/MyGUIPlugins/${PROJECTNAME}
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
function(mygui_config_plugin PROJECTNAME)
  mygui_config_common(${PROJECTNAME})
  if (MYGUI_STATIC)
    # add static prefix, if compiling static version
    set_target_properties(${PROJECTNAME} PROPERTIES OUTPUT_NAME ${PROJECTNAME}Static)
  else (MYGUI_STATIC)
    if (CMAKE_COMPILER_IS_GNUCXX)
      # add GCC visibility flags to shared library build
      set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "${MYGUI_GCC_VISIBILITY_FLAGS}")
      # disable "lib" prefix on Unix
      set_target_properties(${PROJECTNAME} PROPERTIES PREFIX "")
	endif (CMAKE_COMPILER_IS_GNUCXX)	
  endif (MYGUI_STATIC)
  mygui_install_target(${PROJECTNAME} ${MYGUI_PLUGIN_PATH})

  if (MYGUI_INSTALL_PDB)
    # install debug pdb files
    if (MYGUI_STATIC)
	  install(FILES ${MYGUI_BINARY_DIR}/lib${MYGUI_LIB_DEBUG_PATH}/${PROJECTNAME}Static_d.pdb
	    DESTINATION lib${MYGUI_LIB_DEBUG_PATH}/opt
		CONFIGURATIONS Debug
	  )
	  install(FILES ${MYGUI_BINARY_DIR}/lib${MYGUI_LIB_RELWDBG_PATH}/${PROJECTNAME}Static.pdb
	    DESTINATION lib${MYGUI_LIB_RELWDBG_PATH}/opt
		CONFIGURATIONS RelWithDebInfo
	  )
	else ()
	  install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_DEBUG_PATH}/${PROJECTNAME}_d.pdb
	    DESTINATION bin${MYGUI_DEBUG_PATH}
		CONFIGURATIONS Debug
	  )
	  install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_RELWDBG_PATH}/${PROJECTNAME}.pdb
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
