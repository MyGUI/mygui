# Configure settings and install targets

include(GNUInstallDirs)

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

function(mygui_set_platform_name PLATFORM_ID)
	if(${PLATFORM_ID} EQUAL 1)
		set(MYGUI_PLATFORM_NAME Dummy PARENT_SCOPE)
	elseif(${PLATFORM_ID} EQUAL 3)
		set(MYGUI_PLATFORM_NAME Ogre PARENT_SCOPE)
	elseif(${PLATFORM_ID} EQUAL 4)
		set(MYGUI_PLATFORM_NAME OpenGL PARENT_SCOPE)
	elseif(${PLATFORM_ID} EQUAL 5)
		set(MYGUI_PLATFORM_NAME DirectX PARENT_SCOPE)
	elseif(${PLATFORM_ID} EQUAL 6)
		set(MYGUI_PLATFORM_NAME DirectX11 PARENT_SCOPE)
	elseif(${PLATFORM_ID} EQUAL 7)
		set(MYGUI_PLATFORM_NAME OpenGL3 PARENT_SCOPE)
	elseif(${PLATFORM_ID} EQUAL 8)
		set(MYGUI_PLATFORM_NAME OpenGLES PARENT_SCOPE)
	endif()
endfunction(mygui_set_platform_name)

# install targets according to current build type
function(mygui_install_target TARGETNAME SUFFIX)
	install(TARGETS ${TARGETNAME}
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}${MYGUI_RELEASE_PATH}" CONFIGURATIONS Release None ""
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_RELEASE_PATH}${SUFFIX}" CONFIGURATIONS Release None ""
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_RELEASE_PATH}${SUFFIX}" CONFIGURATIONS Release None ""
		FRAMEWORK DESTINATION "${CMAKE_INSTALL_LIBDIR}" CONFIGURATIONS Release None ""
	)
	install(TARGETS ${TARGETNAME}
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}${MYGUI_RELWDBG_PATH}" CONFIGURATIONS RelWithDebInfo
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_RELWDBG_PATH}${SUFFIX}" CONFIGURATIONS RelWithDebInfo
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_RELWDBG_PATH}${SUFFIX}" CONFIGURATIONS RelWithDebInfo
		FRAMEWORK DESTINATION "${CMAKE_INSTALL_LIBDIR}" CONFIGURATIONS RelWithDebInfo
	)
	install(TARGETS ${TARGETNAME}
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}${MYGUI_MINSIZE_PATH}" CONFIGURATIONS MinSizeRel
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_MINSIZE_PATH}${SUFFIX}" CONFIGURATIONS MinSizeRel
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_MINSIZE_PATH}${SUFFIX}" CONFIGURATIONS MinSizeRel
		FRAMEWORK DESTINATION "${CMAKE_INSTALL_LIBDIR}" CONFIGURATIONS MinSizeRel
	)
	install(TARGETS ${TARGETNAME}
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}${MYGUI_DEBUG_PATH}" CONFIGURATIONS Debug
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_DEBUG_PATH}${SUFFIX}" CONFIGURATIONS Debug
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_DEBUG_PATH}${SUFFIX}" CONFIGURATIONS Debug
		FRAMEWORK DESTINATION "${CMAKE_INSTALL_LIBDIR}" CONFIGURATIONS Debug
	)
endfunction(mygui_install_target)

# setup common target settings
function(mygui_config_common TARGETNAME)
	set_target_properties(${TARGETNAME} PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
		LIBRARY_OUTPUT_DIRECTORY ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
		RUNTIME_OUTPUT_DIRECTORY ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}
	)
endfunction(mygui_config_common)

#setup Demo builds
function(mygui_app PROJECTNAME SOLUTIONFOLDER)
	include_directories(
		.
		${MYGUI_SOURCE_DIR}/MyGUIEngine/include
	)
	# define the sources
	include(${PROJECTNAME}.list)

	# setup demo target
	if (${SOLUTIONFOLDER} STREQUAL "Wrappers")
		add_library(${PROJECTNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
	else ()
		# determine specific executable type
		if (WIN32)
			set(MYGUI_EXEC_TYPE WIN32)
		endif ()
		add_executable(${PROJECTNAME} ${MYGUI_EXEC_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
	endif ()
	set_target_properties(${PROJECTNAME} PROPERTIES FOLDER ${SOLUTIONFOLDER})

	mygui_config_sample(${PROJECTNAME})

	target_link_libraries(${PROJECTNAME}
		MyGUICommon
	)

	mygui_set_platform_name(${MYGUI_RENDERSYSTEM})
	target_link_libraries(${PROJECTNAME} MyGUI.${MYGUI_PLATFORM_NAME}Platform)

	if (MYGUI_GENERATE_LIST_FILES_FROM_VSPROJECT)
		add_custom_command(TARGET ${PROJECTNAME}
			POST_BUILD
			COMMAND ${MYGUI_BINARY_DIR}/updateListFiles.bat
			COMMENT "Generating *.list files")
	endif ()

endfunction(mygui_app)

#setup Tools dll builds
function(mygui_dll PROJECTNAME SOLUTIONFOLDER)
	include_directories(
		.
		${MYGUI_SOURCE_DIR}/Common
		${MYGUI_SOURCE_DIR}/MyGUIEngine/include
	)
	# define the sources
	include(${PROJECTNAME}.list)

	add_definitions("-D_USRDLL -DMYGUI_BUILD_DLL")
	add_library(${PROJECTNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
	set_target_properties(${PROJECTNAME} PROPERTIES FOLDER ${SOLUTIONFOLDER})

	mygui_config_lib(${PROJECTNAME})

	mygui_config_sample(${PROJECTNAME})

	target_link_libraries(${PROJECTNAME}
		MyGUICommon
	)

	mygui_set_platform_name(${MYGUI_RENDERSYSTEM})
	target_link_libraries(${PROJECTNAME} MyGUI.${MYGUI_PLATFORM_NAME}Platform)

	if (MYGUI_GENERATE_LIST_FILES_FROM_VSPROJECT)
		add_custom_command(TARGET ${PROJECTNAME}
			POST_BUILD
			COMMAND ${MYGUI_BINARY_DIR}/updateListFiles.bat
			COMMENT "Generating *.list files")
	endif ()

endfunction(mygui_dll)

function(mygui_demo PROJECTNAME)
	mygui_app(${PROJECTNAME} Demos)
	if (MYGUI_INSTALL_DEMOS)
		mygui_install_app(${PROJECTNAME})
	endif ()
endfunction(mygui_demo)


function(mygui_tool PROJECTNAME)
	mygui_app(${PROJECTNAME} Tools)
	if (MYGUI_INSTALL_TOOLS)
		mygui_install_app(${PROJECTNAME})
	endif ()

	include_directories(${MYGUI_SOURCE_DIR}/Tools/EditorFramework)
	if (NOT MYGUI_CLANG_TIDY_BUILD)
		target_precompile_headers(${PROJECTNAME} PRIVATE "../../Common/Precompiled.h")
	else ()
		target_compile_options(${PROJECTNAME} PRIVATE -include "${CMAKE_CURRENT_LIST_DIR}/../../Common/Precompiled.h")
	endif ()

	target_link_libraries(${PROJECTNAME}
		EditorFramework
	)
endfunction(mygui_tool)


function(mygui_unit_test PROJECTNAME)
	mygui_app(${PROJECTNAME} UnitTest)

	# Register the unit test with CTest when unit tests are enabled
	# This allows running tests via 'ctest' or 'make check'
	if (MYGUI_BUILD_UNITTESTS)
		add_test(NAME ${PROJECTNAME} COMMAND ${PROJECTNAME})
		# Set working directory to binary directory so tests can find resources
		set_tests_properties(${PROJECTNAME} PROPERTIES
			WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	endif()
endfunction(mygui_unit_test)


function(mygui_tool_dll PROJECTNAME)
	mygui_dll(${PROJECTNAME} Tools)

	if (NOT MYGUI_CLANG_TIDY_BUILD)
		target_precompile_headers(${PROJECTNAME} PRIVATE "../../Common/Precompiled.h")
	else ()
		target_compile_options(${PROJECTNAME} PRIVATE -include "${CMAKE_CURRENT_LIST_DIR}/../../Common/Precompiled.h")
	endif ()
endfunction(mygui_tool_dll)


function(mygui_install_app PROJECTNAME)
	if (MYGUI_INSTALL_PDB)
		install(FILES ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}${MYGUI_DEBUG_PATH}/${PROJECTNAME}.pdb
			DESTINATION ${CMAKE_INSTALL_BINDIR}${MYGUI_DEBUG_PATH}
			CONFIGURATIONS Debug
		)
		install(FILES ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}${MYGUI_RELWDBG_PATH}/${PROJECTNAME}.pdb
			DESTINATION ${CMAKE_INSTALL_BINDIR}${MYGUI_RELWDBG_PATH}
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
	set_target_properties(${PROJECTNAME} PROPERTIES FOLDER "Plugins")
	target_link_libraries(${PROJECTNAME} MyGUIEngine)

	mygui_config_lib(${PROJECTNAME})

	# Plugins are loaded at runtime and not linked at buildtime, so they should go to the same
	# output directory as the executables, so the plugin loader can find them there.
	set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

	# Remove default library prefix (e.g. "lib" on Linux), as the plugin loader doesn't know about it
	set_target_properties(${PROJECTNAME} PROPERTIES PREFIX "")

	install(FILES ${HEADER_FILES}
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
	)
endfunction(mygui_plugin)


# setup library build
function(mygui_config_lib PROJECTNAME)
	mygui_config_common(${PROJECTNAME})
	if (MYGUI_STATIC)
		# add static prefix, if compiling static version
		set_target_properties(${PROJECTNAME} PROPERTIES OUTPUT_NAME ${PROJECTNAME}Static)
	else (MYGUI_STATIC)
		target_compile_definitions(${PROJECTNAME} PRIVATE MYGUI_GCC_VISIBILITY)
	endif (MYGUI_STATIC)
	mygui_install_target(${PROJECTNAME} "")

	if (MYGUI_INSTALL_PDB AND NOT MYGUI_STATIC)
		install(FILES ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}${MYGUI_DEBUG_PATH}/${PROJECTNAME}_d.pdb
			DESTINATION ${CMAKE_INSTALL_BINDIR}${MYGUI_DEBUG_PATH}
			CONFIGURATIONS Debug
		)
		install(FILES ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}${MYGUI_RELWDBG_PATH}/${PROJECTNAME}.pdb
			DESTINATION ${CMAKE_INSTALL_BINDIR}${MYGUI_RELWDBG_PATH}
			CONFIGURATIONS RelWithDebInfo
		)
	endif ()
endfunction(mygui_config_lib)


# setup demo build
function(mygui_config_sample PROJECTNAME)
	mygui_config_common(${PROJECTNAME})

	# set install RPATH for Unix systems
	if (UNIX AND MYGUI_FULL_RPATH)
		set_property(TARGET ${PROJECTNAME} APPEND PROPERTY
			INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
		set_property(TARGET ${PROJECTNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
	endif ()
endfunction(mygui_config_sample)
