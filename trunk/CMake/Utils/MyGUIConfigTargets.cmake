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

function(mygui_add_base_manager_source PLATFORM)
	include_directories(../../Common/Base/${PLATFORM})
endfunction(mygui_add_base_manager_source)

function(mygui_add_input_source PLATFORM)
	include_directories(../../Common/Input/${PLATFORM})
endfunction(mygui_add_input_source)

#setup Demo builds
function(mygui_app PROJECTNAME SOLUTIONFOLDER)
	include_directories(
		.
		${MYGUI_SOURCE_DIR}/Common
		${MYGUI_SOURCE_DIR}/MyGUIEngine/include
	)
	# define the sources
	include(${PROJECTNAME}.list)
	if(MYGUI_RENDERSYSTEM EQUAL 1)
		include_directories(../../Common/Base/DirectX)
		add_definitions("-DMYGUI_DIRECTX_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/DirectX/DirectXPlatform/include
			${DirectX_INCLUDE_DIR}
		)
		link_directories(${DIRECTX_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 2)
		include_directories(../../Common/Base/Ogre)
		add_definitions("-DMYGUI_OGRE_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/Ogre/OgrePlatform/include
			${OGRE_INCLUDE_DIR}
		)
		link_directories(${OGRE_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 3)
		include_directories(../../Common/Base/OpenGL)
		add_definitions("-DMYGUI_OPENGL_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/OpenGL/OpenGLPlatform/include
			${OPENGL_INCLUDE_DIR}
		)
		link_directories(${OPENGL_LIB_DIR})
	endif()
	
	if(MYGUI_SAMPLES_INPUT EQUAL 1)
		add_definitions("-DMYGUI_SAMPLES_INPUT_OIS")
		include_directories(../../Common/Input/OIS)
		include_directories(${OIS_INCLUDE_DIRS})
	elseif(MYGUI_SAMPLES_INPUT EQUAL 2)
		add_definitions("-DMYGUI_SAMPLES_INPUT_WIN32")
		include_directories(../../Common/Input/Win32)
	elseif(MYGUI_SAMPLES_INPUT EQUAL 3)
		add_definitions("-DMYGUI_SAMPLES_INPUT_WIN32_OIS")
		include_directories(../../Common/Input/Win32_OIS)
		include_directories(${OIS_INCLUDE_DIRS})
	endif()
	
	if (MYGUI_DONT_USE_OBSOLETE)
		add_definitions(-DMYGUI_DONT_USE_OBSOLETE)
	endif ()
	
	# setup demo target
	if (${SOLUTIONFOLDER} STREQUAL "Wrappers")
		add_library(${PROJECTNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
	else ()
		add_executable(${PROJECTNAME} WIN32 ${HEADER_FILES} ${SOURCE_FILES})
	endif ()
	set_target_properties(${PROJECTNAME} PROPERTIES PROJECT_GROUP ${SOLUTIONFOLDER})
	
	add_dependencies(${PROJECTNAME} MyGUIEngine Common)

	mygui_config_sample(${PROJECTNAME})
	
	target_link_libraries(${PROJECTNAME}
		MyGUIEngine
		Common
		uuid
	)
endfunction(mygui_app)


function(mygui_demo PROJECTNAME)
	mygui_app(${PROJECTNAME} Demos)
	if (MYGUI_INSTALL_SAMPLES)
		mygui_install_app(${PROJECTNAME})
	endif ()
endfunction(mygui_demo)


function(mygui_tool PROJECTNAME)
	mygui_app(${PROJECTNAME} Tools)
	if (MYGUI_INSTALL_TOOLS)
		mygui_install_app(${PROJECTNAME})
	endif ()
endfunction(mygui_tool)


function(mygui_unit_test PROJECTNAME)
	mygui_app(${PROJECTNAME} UnitTest)
endfunction(mygui_unit_test)


#function(mygui_wrapper_base_app PROJECTNAME)
#	mygui_app(${PROJECTNAME} Wrappers)
#endfunction(mygui_wrapper_base_app)


function(mygui_install_app PROJECTNAME)
	if (MYGUI_INSTALL_PDB)
		# install debug pdb files
		install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_DEBUG_PATH}/${PROJECTNAME}.pdb
			DESTINATION bin${MYGUI_DEBUG_PATH}
			CONFIGURATIONS Debug
		)
		install(FILES ${MYGUI_BINARY_DIR}/bin${MYGUI_RELWDBG_PATH}/${PROJECTNAME}.pdb
			DESTINATION bin${MYGUI_RELWDBG_PATH}
			CONFIGURATIONS RelWithDebInfo
		)
	endif ()
	
	mygui_install_target(${PROJECTNAME} "")
endfunction(mygui_install_app)


#setup Plugin builds
function(mygui_plugin PROJECTNAME)
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
endfunction(mygui_plugin)


# setup library build
function(mygui_config_lib PROJECTNAME)
	mygui_config_common(${PROJECTNAME})
	if (MYGUI_STATIC)
		# add static prefix, if compiling static version
		set_target_properties(${PROJECTNAME} PROPERTIES OUTPUT_NAME ${PROJECTNAME}Static)
	else (MYGUI_STATIC)
		if (CMAKE_COMPILER_IS_GNUCXX)
			# add GCC visibility flags to shared library build
			set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "${MYGUI_GCC_VISIBILITY_FLAGS}")
		endif (CMAKE_COMPILER_IS_GNUCXX)
	endif (MYGUI_STATIC)
	mygui_install_target(${PROJECTNAME} "")
	
	if (MYGUI_INSTALL_PDB)
		# install debug pdb files
		if (MYGUI_STATIC)
			install(FILES ${MYGUI_BINARY_DIR}/lib${MYGUI_LIB_DEBUG_PATH}/${PROJECTNAME}Static_d.pdb
				DESTINATION lib${MYGUI_LIB_DEBUG_PATH}
				CONFIGURATIONS Debug
			)
			install(FILES ${MYGUI_BINARY_DIR}/lib${MYGUI_LIB_RELWDBG_PATH}/${PROJECTNAME}Static.pdb
				DESTINATION lib${MYGUI_LIB_RELWDBG_PATH}
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
endfunction(mygui_config_lib)


# setup demo build
function(mygui_config_sample PROJECTNAME)
	mygui_config_common(${PROJECTNAME})

	# set install RPATH for Unix systems
	if (UNIX AND MYGUI_FULL_RPATH)
		set_property(TARGET ${PROJECTNAME} APPEND PROPERTY
			INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
		set_property(TARGET ${PROJECTNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
	endif ()
endfunction(mygui_config_sample)
