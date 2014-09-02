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
if (APPLE)
	set(MYGUI_FRAMEWORK_PATH /Library/Frameworks)
endif ()

# create vcproj.user file for Visual Studio to set debug working directory
function(mygui_create_vcproj_userfile TARGETNAME)
	if (MSVC)
		# for VisualStudioUserFile.vcproj.user.in
		if(CMAKE_CL_64)
			set(MYGUI_WIN_BUILD_CONFIGURATION "x64")
		else()
			set(MYGUI_WIN_BUILD_CONFIGURATION "Win32")
		endif()
		
		#FIXME
		if (${CMAKE_GENERATOR} STREQUAL "Visual Studio 10" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10 Win64" OR
			${CMAKE_GENERATOR} STREQUAL "Visual Studio 11" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 11 Win64" OR
			${CMAKE_GENERATOR} STREQUAL "Visual Studio 12" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 Win64"
			)
			configure_file(
				${MYGUI_TEMPLATES_DIR}/VisualStudio2010UserFile.vcxproj.user.in
				${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}.vcxproj.user
				@ONLY
			)
		else ()
			configure_file(
				${MYGUI_TEMPLATES_DIR}/VisualStudioUserFile.vcproj.user.in
				${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}.vcproj.user
				@ONLY
			)
		endif ()
		
	endif ()
endfunction(mygui_create_vcproj_userfile)

# install targets according to current build type
function(mygui_install_target TARGETNAME SUFFIX)
	install(TARGETS ${TARGETNAME}
		RUNTIME DESTINATION "bin${MYGUI_RELEASE_PATH}" CONFIGURATIONS Release None ""
		LIBRARY DESTINATION "lib${MYGUI_LIB_RELEASE_PATH}${SUFFIX}" CONFIGURATIONS Release None ""
		ARCHIVE DESTINATION "lib${MYGUI_LIB_RELEASE_PATH}${SUFFIX}" CONFIGURATIONS Release None ""
		FRAMEWORK DESTINATION "${MYGUI_FRAMEWORK_PATH}" CONFIGURATIONS Release None ""
	)
	install(TARGETS ${TARGETNAME}
		RUNTIME DESTINATION "bin${MYGUI_RELWDBG_PATH}" CONFIGURATIONS RelWithDebInfo
		LIBRARY DESTINATION "lib${MYGUI_LIB_RELWDBG_PATH}${SUFFIX}" CONFIGURATIONS RelWithDebInfo
		ARCHIVE DESTINATION "lib${MYGUI_LIB_RELWDBG_PATH}${SUFFIX}" CONFIGURATIONS RelWithDebInfo
		FRAMEWORK DESTINATION "${MYGUI_FRAMEWORK_PATH}" CONFIGURATIONS RelWithDebInfo
	)
	install(TARGETS ${TARGETNAME}
		RUNTIME DESTINATION "bin${MYGUI_MINSIZE_PATH}" CONFIGURATIONS MinSizeRel
		LIBRARY DESTINATION "lib${MYGUI_LIB_MINSIZE_PATH}${SUFFIX}" CONFIGURATIONS MinSizeRel
		ARCHIVE DESTINATION "lib${MYGUI_LIB_MINSIZE_PATH}${SUFFIX}" CONFIGURATIONS MinSizeRel
		FRAMEWORK DESTINATION "${MYGUI_FRAMEWORK_PATH}" CONFIGURATIONS MinSizeRel
	)
	install(TARGETS ${TARGETNAME}
		RUNTIME DESTINATION "bin${MYGUI_DEBUG_PATH}" CONFIGURATIONS Debug
		LIBRARY DESTINATION "lib${MYGUI_LIB_DEBUG_PATH}${SUFFIX}" CONFIGURATIONS Debug
		ARCHIVE DESTINATION "lib${MYGUI_LIB_DEBUG_PATH}${SUFFIX}" CONFIGURATIONS Debug
		FRAMEWORK DESTINATION "${MYGUI_FRAMEWORK_PATH}" CONFIGURATIONS Debug
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
function(mygui_app PROJECTNAME SOLUTIONFOLDER)
	include_directories(
		.
		${MYGUI_SOURCE_DIR}/Common
		${MYGUI_SOURCE_DIR}/MyGUIEngine/include
	)
	# define the sources
	include(${PROJECTNAME}.list)
	
	# Set up dependencies
	if(MYGUI_RENDERSYSTEM EQUAL 1)
		include_directories(../../Common/Base/Dummy)
		add_definitions("-DMYGUI_DUMMY_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/Dummy/DummyPlatform/include
		)
	elseif(MYGUI_RENDERSYSTEM EQUAL 3)
		include_directories(../../Common/Base/Ogre)
		add_definitions("-DMYGUI_OGRE_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/Ogre/OgrePlatform/include
			${OGRE_INCLUDE_DIR}
		)
		link_directories(${OGRE_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 4)
		include_directories(../../Common/Base/OpenGL)
		add_definitions("-DMYGUI_OPENGL_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/OpenGL/OpenGLPlatform/include
			${OPENGL_INCLUDE_DIR}
		)
		link_directories(${OPENGL_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 5)
		include_directories(../../Common/Base/DirectX)
		add_definitions("-DMYGUI_DIRECTX_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/DirectX/DirectXPlatform/include
			${DirectX_INCLUDE_DIR}
		)
		link_directories(${DIRECTX_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 6)
		include_directories(../../Common/Base/DirectX11)
		add_definitions("-DMYGUI_DIRECTX11_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/DirectX11/DirectX11Platform/include
			${DirectX_INCLUDE_DIR}
		)
		link_directories(${DIRECTX_LIB_DIR})
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
	
	# setup demo target
	if (${SOLUTIONFOLDER} STREQUAL "Wrappers")
		add_library(${PROJECTNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
	else ()
		# determine specific executable type
		if (APPLE)
			set(MYGUI_EXEC_TYPE MACOSX_BUNDLE)
		elseif (WIN32)
			set(MYGUI_EXEC_TYPE WIN32)
		endif ()
		add_executable(${PROJECTNAME} ${MYGUI_EXEC_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
	endif ()
	set_target_properties(${PROJECTNAME} PROPERTIES FOLDER ${SOLUTIONFOLDER})
	
	add_dependencies(${PROJECTNAME} MyGUIEngine Common)

	mygui_config_sample(${PROJECTNAME})
	
	# link Common, Platform and MyGUIEngine
	target_link_libraries(${PROJECTNAME}
		Common
	)
	if(MYGUI_RENDERSYSTEM EQUAL 5)
		add_dependencies(${PROJECTNAME} MyGUI.DirectXPlatform)
		target_link_libraries(${PROJECTNAME} MyGUI.DirectXPlatform)
	elseif(MYGUI_RENDERSYSTEM EQUAL 3)
		add_dependencies(${PROJECTNAME} MyGUI.OgrePlatform)
		target_link_libraries(${PROJECTNAME} MyGUI.OgrePlatform)
	elseif(MYGUI_RENDERSYSTEM EQUAL 4)
		add_dependencies(${PROJECTNAME} MyGUI.OpenGLPlatform)
		target_link_libraries(${PROJECTNAME} MyGUI.OpenGLPlatform)
		
		target_link_libraries(${PROJECTNAME} gdiplus)
	endif()
	target_link_libraries(${PROJECTNAME}
		MyGUIEngine
	)

	if (MYGUI_GENERATE_LIST_FILES_FROM_VSPROJECT)
		add_custom_command(TARGET ${PROJECTNAME}
			POST_BUILD
			COMMAND ${MYGUI_BINARY_DIR}/updateListFiles.bat
			COMMENT "Generating *.list files")
	endif ()

	if (APPLE)
		find_library(CF_LIBRARY CoreFoundation)
		find_library(IOKIT_LIBRARY IOKit)
		target_link_libraries(${PROJECTNAME} ${CF_LIBRARY})
		target_link_libraries(${PROJECTNAME} ${IOKIT_LIBRARY})
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

	# Set up dependencies
	if(MYGUI_RENDERSYSTEM EQUAL 1)
		include_directories(../../Common/Base/Dummy)
		add_definitions("-DMYGUI_DUMMY_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/Dummy/DummyPlatform/include
		)
	elseif(MYGUI_RENDERSYSTEM EQUAL 3)
		include_directories(../../Common/Base/Ogre)
		add_definitions("-DMYGUI_OGRE_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/Ogre/OgrePlatform/include
			${OGRE_INCLUDE_DIR}
		)
		link_directories(${OGRE_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 4)
		include_directories(../../Common/Base/OpenGL)
		add_definitions("-DMYGUI_OPENGL_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/OpenGL/OpenGLPlatform/include
			${OPENGL_INCLUDE_DIR}
		)
		link_directories(${OPENGL_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 5)
		include_directories(../../Common/Base/DirectX)
		add_definitions("-DMYGUI_DIRECTX_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/DirectX/DirectXPlatform/include
			${DirectX_INCLUDE_DIR}
		)
		link_directories(${DIRECTX_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 6)
		include_directories(../../Common/Base/DirectX11)
		add_definitions("-DMYGUI_DIRECTX11_PLATFORM")
		include_directories(
			${MYGUI_SOURCE_DIR}/Platforms/DirectX11/DirectX11Platform/include
			${DirectX_INCLUDE_DIR}
		)
		link_directories(${DIRECTX_LIB_DIR})
	endif()
	
		
	add_definitions("-D_USRDLL -DMYGUI_BUILD_DLL")
	add_library(${PROJECTNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
	set_target_properties(${PROJECTNAME} PROPERTIES FOLDER ${SOLUTIONFOLDER})
	add_dependencies(${PROJECTNAME} MyGUIEngine)
	target_link_libraries(${PROJECTNAME} MyGUIEngine)
	
	mygui_config_lib(${PROJECTNAME})
	
	add_dependencies(${PROJECTNAME} MyGUIEngine Common)

	mygui_config_sample(${PROJECTNAME})
	
	target_link_libraries(${PROJECTNAME}
		Common
	)

	if(MYGUI_RENDERSYSTEM EQUAL 5)
		add_dependencies(${PROJECTNAME} MyGUI.DirectXPlatform)
		target_link_libraries(${PROJECTNAME} MyGUI.DirectXPlatform)
	elseif(MYGUI_RENDERSYSTEM EQUAL 3)
		add_dependencies(${PROJECTNAME} MyGUI.OgrePlatform)
		target_link_libraries(${PROJECTNAME} MyGUI.OgrePlatform)
	elseif(MYGUI_RENDERSYSTEM EQUAL 4)
		add_dependencies(${PROJECTNAME} MyGUI.OpenGLPlatform)
		target_link_libraries(${PROJECTNAME} MyGUI.OpenGLPlatform)
		
		target_link_libraries(${PROJECTNAME} gdiplus)
	endif()

	target_link_libraries(${PROJECTNAME}
		MyGUIEngine
	)

	if (MYGUI_GENERATE_LIST_FILES_FROM_VSPROJECT)
		add_custom_command(TARGET ${PROJECTNAME}
			POST_BUILD
			COMMAND ${MYGUI_BINARY_DIR}/updateListFiles.bat
			COMMENT "Generating *.list files")
	endif ()

	if (APPLE)
		find_library(CF_LIBRARY CoreFoundation)
		find_library(IOKIT_LIBRARY IOKit)
		target_link_libraries(${PROJECTNAME} ${CF_LIBRARY})
		target_link_libraries(${PROJECTNAME} ${IOKIT_LIBRARY})
	endif ()
endfunction(mygui_dll)

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

	include_directories(${MYGUI_SOURCE_DIR}/Tools/EditorFramework)
	include(PrecompiledHeader)
	# specify a precompiled header to use
	use_precompiled_header(${PROJECTNAME}
		"../../Common/Precompiled.h"
		"../../Common/Precompiled.cpp"
	)

	target_link_libraries(${PROJECTNAME}
		EditorFramework
	)
endfunction(mygui_tool)


function(mygui_unit_test PROJECTNAME)
	mygui_app(${PROJECTNAME} UnitTest)
endfunction(mygui_unit_test)


function(mygui_tool_dll PROJECTNAME)
	mygui_dll(${PROJECTNAME} Tools)
	if (MYGUI_INSTALL_TOOLS)
		mygui_install_app(${PROJECTNAME})
	endif ()

	include(PrecompiledHeader)
	# specify a precompiled header to use
	use_precompiled_header(${PROJECTNAME}
		"../../Common/Precompiled.h"
		"../../Common/Precompiled.cpp"
	)
endfunction(mygui_tool_dll)


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
	set_target_properties(${PROJECTNAME} PROPERTIES FOLDER "Plugins")
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
			if (APPLE)
				# deal with Mac OS X's framework system
				set_target_properties(${PROJECTNAME} PROPERTIES FRAMEWORK TRUE)
				set_target_properties(${PROJECTNAME} PROPERTIES PUBLIC_HEADER "${${PROJECTNAME}_HEADERS}")
				set_target_properties(${PROJECTNAME} PROPERTIES OUTPUT_NAME ${PROJECTNAME})
				set_target_properties(${PROJECTNAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
				set_target_properties(${PROJECTNAME} PROPERTIES INSTALL_NAME_DIR "@executable_path/../Frameworks")
			endif (APPLE)
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