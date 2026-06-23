# Configure settings and install targets

include(GNUInstallDirs)

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
function(mygui_install_target TARGETNAME)
	install(
		TARGETS ${TARGETNAME}
		EXPORT MyGUITargets
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		FRAMEWORK DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	)
endfunction(mygui_install_target)

# setup common target settings
function(mygui_config_common TARGETNAME)
	set_target_properties(${TARGETNAME} PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
		LIBRARY_OUTPUT_DIRECTORY "${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
		RUNTIME_OUTPUT_DIRECTORY "${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}"
	)
endfunction(mygui_config_common)

#setup Demo builds
function(mygui_app PROJECTNAME SOLUTIONFOLDER)
	# define the sources
	include(${PROJECTNAME}.list)

	# setup demo target
	if (${SOLUTIONFOLDER} STREQUAL "Wrappers")
		add_library(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})
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
		PRIVATE
			MyGUICommon
	)
endfunction(mygui_app)

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

	target_include_directories(${PROJECTNAME} PRIVATE "${MYGUI_SOURCE_DIR}/Tools/EditorFramework")
	mygui_target_precompile_headers(${PROJECTNAME} "../../Common/Precompiled.h")

	target_link_libraries(${PROJECTNAME} PRIVATE EditorFramework)
endfunction(mygui_tool)


function(mygui_unit_test PROJECTNAME)
	mygui_app(${PROJECTNAME} UnitTest)

	# Register the unit test with CTest when unit tests are enabled
	# This allows running tests via 'ctest' or 'make check'
	if (MYGUI_BUILD_UNITTESTS)
		add_test(NAME ${PROJECTNAME} COMMAND ${PROJECTNAME})
		# Set working directory to binary directory so tests can find resources
		set_tests_properties(${PROJECTNAME} PROPERTIES
			WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
	endif()
endfunction(mygui_unit_test)


function(mygui_tool_dll PROJECTNAME)
	include(${PROJECTNAME}.list)

	if(MYGUI_RENDERSYSTEM EQUAL 3 AND OGRE_STATIC)
		add_library(${PROJECTNAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})
	else()
		add_library(${PROJECTNAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
		target_compile_definitions(${PROJECTNAME} PRIVATE _USRDLL MYGUI_BUILD_DLL)
	endif()

	set_target_properties(${PROJECTNAME} PROPERTIES FOLDER Tools)

	mygui_config_lib(${PROJECTNAME})
	mygui_config_sample(${PROJECTNAME})
	install(TARGETS EditorFramework
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		FRAMEWORK DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	)
	target_link_libraries(${PROJECTNAME}
		PRIVATE
			MyGUICommon
	)

	mygui_target_precompile_headers(${PROJECTNAME} "../../Common/Precompiled.h")
endfunction(mygui_tool_dll)


function(mygui_install_app PROJECTNAME)
	if (MYGUI_INSTALL_PDB)
		install(FILES "${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/${PROJECTNAME}.pdb"
			DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
	endif ()

	install(
		TARGETS ${PROJECTNAME}
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		FRAMEWORK DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	)
endfunction(mygui_install_app)


#setup Plugin builds
function(mygui_plugin PROJECTNAME)
	# define the sources
	include(${PROJECTNAME}.list)

	add_library(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})
	target_compile_definitions(${PROJECTNAME} PRIVATE _USRDLL MYGUI_BUILD_DLL)
	set_target_properties(${PROJECTNAME} PROPERTIES FOLDER "Plugins")
	target_link_libraries(${PROJECTNAME} PUBLIC MyGUI::MyGUI)

	mygui_config_lib(${PROJECTNAME})

	# Plugins are loaded at runtime and not linked at buildtime, so they should go to the same
	# output directory as the executables, so the plugin loader can find them there.
	set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

	# Remove default library prefix (e.g. "lib" on Linux), as the plugin loader doesn't know about it
	set_target_properties(${PROJECTNAME} PROPERTIES PREFIX "")

	install(FILES ${HEADER_FILES}
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
	)
endfunction(mygui_plugin)


# setup library build
function(mygui_config_lib PROJECTNAME)
	mygui_config_common(${PROJECTNAME})
	if (NOT BUILD_SHARED_LIBS)
		# add static prefix, if compiling static version
		set_target_properties(${PROJECTNAME} PROPERTIES OUTPUT_NAME ${PROJECTNAME}Static)
	else()
		target_compile_definitions(${PROJECTNAME} PRIVATE MYGUI_GCC_VISIBILITY)
	endif()

	if (MYGUI_INSTALL_PDB)
		install(FILES "${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/${PROJECTNAME}.pdb"
			DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
	endif ()
endfunction(mygui_config_lib)


# setup demo build
function(mygui_config_sample PROJECTNAME)
	mygui_config_common(${PROJECTNAME})

	# set install RPATH for Unix systems
	if (UNIX AND MYGUI_FULL_RPATH)
		set_property(TARGET ${PROJECTNAME} APPEND PROPERTY
			INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
		set_property(TARGET ${PROJECTNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
	endif ()
endfunction(mygui_config_sample)


function(mygui_target_precompile_headers PROJECTNAME HEADER)
	if (NOT MYGUI_CLANG_TIDY_BUILD)
		target_precompile_headers(${PROJECTNAME} PRIVATE "${HEADER}")
	else ()
		if(IS_ABSOLUTE "${HEADER}")
			target_compile_options(${PROJECTNAME} PRIVATE -include "${HEADER}")
		else()
			target_compile_options(${PROJECTNAME} PRIVATE -include "${CMAKE_CURRENT_LIST_DIR}/${HEADER}")
		endif()
	endif ()
endfunction()
