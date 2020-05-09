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
  if (NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(MYGUI_FRAMEWORK_PATH ${CMAKE_INSTALL_LIBDIR}/${CMAKE_BUILD_TYPE})
  else ()
    set(MYGUI_FRAMEWORK_PATH /Library/Frameworks)
  endif ()
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
		FRAMEWORK DESTINATION "${MYGUI_FRAMEWORK_PATH}" CONFIGURATIONS Release None ""
	)
	install(TARGETS ${TARGETNAME}
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}${MYGUI_RELWDBG_PATH}" CONFIGURATIONS RelWithDebInfo
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_RELWDBG_PATH}${SUFFIX}" CONFIGURATIONS RelWithDebInfo
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_RELWDBG_PATH}${SUFFIX}" CONFIGURATIONS RelWithDebInfo
		FRAMEWORK DESTINATION "${MYGUI_FRAMEWORK_PATH}" CONFIGURATIONS RelWithDebInfo
	)
	install(TARGETS ${TARGETNAME}
		RUNTIME DESTINATION "bin${MYGUI_MINSIZE_PATH}" CONFIGURATIONS MinSizeRel
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_MINSIZE_PATH}${SUFFIX}" CONFIGURATIONS MinSizeRel
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_MINSIZE_PATH}${SUFFIX}" CONFIGURATIONS MinSizeRel
		FRAMEWORK DESTINATION "${MYGUI_FRAMEWORK_PATH}" CONFIGURATIONS MinSizeRel
	)
	install(TARGETS ${TARGETNAME}
		RUNTIME DESTINATION "bin${MYGUI_DEBUG_PATH}" CONFIGURATIONS Debug
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_DEBUG_PATH}${SUFFIX}" CONFIGURATIONS Debug
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_DEBUG_PATH}${SUFFIX}" CONFIGURATIONS Debug
		FRAMEWORK DESTINATION "${MYGUI_FRAMEWORK_PATH}" CONFIGURATIONS Debug
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
		${MYGUI_SOURCE_DIR}/Common
		${MYGUI_SOURCE_DIR}/MyGUIEngine/include
		${MYGUI_SOURCE_DIR}/Common/Input/SDL
	)
	# define the sources
	include(${PROJECTNAME}.list)

	include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS}/..)
	link_directories(${SDL2_LIB_DIR})

	# Set up dependencies
	mygui_add_base_manager_include(${MYGUI_RENDERSYSTEM})
	mygui_set_platform_name(${MYGUI_RENDERSYSTEM})
	include_directories(
		${MYGUI_SOURCE_DIR}/Platforms/${MYGUI_PLATFORM_NAME}/${MYGUI_PLATFORM_NAME}Platform/include
	)
	if(MYGUI_RENDERSYSTEM EQUAL 1)
		add_definitions("-DMYGUI_DUMMY_PLATFORM")
	elseif(MYGUI_RENDERSYSTEM EQUAL 3)
		add_definitions("-DMYGUI_OGRE_PLATFORM")
		include_directories(
			${OGRE_INCLUDE_DIR}
		)
		link_directories(${OGRE_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 4)
		add_definitions("-DMYGUI_OPENGL_PLATFORM")
		include_directories(
			${OPENGL_INCLUDE_DIR}
			${SDL2_IMAGE_INCLUDE_DIRS}
		)
		link_directories(
			${OPENGL_LIB_DIR}
			${SDL2_IMAGE_LIB_DIR}
		)

	elseif(MYGUI_RENDERSYSTEM EQUAL 5)
		add_definitions("-DMYGUI_DIRECTX_PLATFORM")
		include_directories(
			${DirectX_INCLUDE_DIR}
		)
		link_directories(${DIRECTX_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 6)
		add_definitions("-DMYGUI_DIRECTX11_PLATFORM")
		include_directories(
			${DirectX_INCLUDE_DIR}
		)
		link_directories(${DIRECTX_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 7)
		add_definitions("-DMYGUI_OPENGL3_PLATFORM")
		include_directories(
			${OPENGL_INCLUDE_DIR}
			${SDL2_IMAGE_INCLUDE_DIRS}
		)
		link_directories(
			${OPENGL_LIB_DIR}
			${SDL2_IMAGE_LIB_DIR}
		)
	elseif(MYGUI_RENDERSYSTEM EQUAL 8)
		add_definitions("-DMYGUI_OPENGLES_PLATFORM")
		include_directories(
			${OPENGL_INCLUDE_DIR}
			${SDL2_IMAGE_INCLUDE_DIRS}
		)
		link_directories(
			${OPENGL_LIB_DIR}
			${SDL2_IMAGE_LIB_DIR}
		)
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
		${SDL2_LIBRARIES}
	)

	mygui_set_platform_name(${MYGUI_RENDERSYSTEM})
	add_dependencies(${PROJECTNAME} MyGUI.${MYGUI_PLATFORM_NAME}Platform)
	target_link_libraries(${PROJECTNAME} MyGUI.${MYGUI_PLATFORM_NAME}Platform)
	if(MYGUI_RENDERSYSTEM EQUAL 1)
	elseif(MYGUI_RENDERSYSTEM EQUAL 3)
	elseif(MYGUI_RENDERSYSTEM EQUAL 4)
		target_link_libraries(${PROJECTNAME} ${SDL2_IMAGE_LIBRARIES})
	elseif(MYGUI_RENDERSYSTEM EQUAL 5)
	elseif(MYGUI_RENDERSYSTEM EQUAL 7)
		target_link_libraries(${PROJECTNAME} ${SDL2_IMAGE_LIBRARIES})
	elseif(MYGUI_RENDERSYSTEM EQUAL 8)
		target_link_libraries(${PROJECTNAME} ${SDL2_IMAGE_LIBRARIES})
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

	include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS}/..)
	link_directories(${SDL2_LIB_DIR})

	# Set up dependencies
	mygui_add_base_manager_include(${MYGUI_RENDERSYSTEM})
	mygui_set_platform_name(${MYGUI_RENDERSYSTEM})
	include_directories(
		${MYGUI_SOURCE_DIR}/Platforms/${MYGUI_PLATFORM_NAME}/${MYGUI_PLATFORM_NAME}Platform/include
	)
	if(MYGUI_RENDERSYSTEM EQUAL 1)
		add_definitions("-DMYGUI_DUMMY_PLATFORM")
	elseif(MYGUI_RENDERSYSTEM EQUAL 3)
		add_definitions("-DMYGUI_OGRE_PLATFORM")
		include_directories(
			${OGRE_INCLUDE_DIR}
		)
		link_directories(${OGRE_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 4)
		add_definitions("-DMYGUI_OPENGL_PLATFORM")
		include_directories(
			${OPENGL_INCLUDE_DIR}
			${SDL2_IMAGE_INCLUDE_DIRS}
		)
		link_directories(
			${OPENGL_LIB_DIR}
			${SDL2_IMAGE_LIB_DIR}
		)
	elseif(MYGUI_RENDERSYSTEM EQUAL 5)
		add_definitions("-DMYGUI_DIRECTX_PLATFORM")
		include_directories(
			${DirectX_INCLUDE_DIR}
		)
		link_directories(
			${DIRECTX_LIB_DIR}
		)
	elseif(MYGUI_RENDERSYSTEM EQUAL 6)
		add_definitions("-DMYGUI_DIRECTX11_PLATFORM")
		include_directories(
			${DirectX_INCLUDE_DIR}
		)
		link_directories(${DIRECTX_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 7)
		add_definitions("-DMYGUI_OPENGL3_PLATFORM")
		include_directories(
			${OPENGL_INCLUDE_DIR}
		)
		link_directories(${OPENGL_LIB_DIR})
	elseif(MYGUI_RENDERSYSTEM EQUAL 8)
		add_definitions("-DMYGUI_OPENGLES_PLATFORM")
		include_directories(
				${OPENGL_INCLUDE_DIR}
		)
		link_directories(${OPENGL_LIB_DIR})
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
		${SDL2_LIBRARIES}
	)

	mygui_set_platform_name(${MYGUI_RENDERSYSTEM})
	add_dependencies(${PROJECTNAME} MyGUI.${MYGUI_PLATFORM_NAME}Platform)
	target_link_libraries(${PROJECTNAME} MyGUI.${MYGUI_PLATFORM_NAME}Platform)
	if(MYGUI_RENDERSYSTEM EQUAL 1)
	elseif(MYGUI_RENDERSYSTEM EQUAL 3)
	elseif(MYGUI_RENDERSYSTEM EQUAL 4)
		target_link_libraries(${PROJECTNAME} ${SDL2_IMAGE_LIBRARIES})
	elseif(MYGUI_RENDERSYSTEM EQUAL 5)
	elseif(MYGUI_RENDERSYSTEM EQUAL 6)
	elseif(MYGUI_RENDERSYSTEM EQUAL 7)
		target_link_libraries(${PROJECTNAME} ${SDL2_IMAGE_LIBRARIES})
	elseif(MYGUI_RENDERSYSTEM EQUAL 8)
		target_link_libraries(${PROJECTNAME} ${SDL2_IMAGE_LIBRARIES})
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
		install(FILES ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}${MYGUI_DEBUG_PATH}/${PROJECTNAME}.pdb
			DESTINATION bin${MYGUI_DEBUG_PATH}
			CONFIGURATIONS Debug
		)
		install(FILES ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}${MYGUI_RELWDBG_PATH}/${PROJECTNAME}.pdb
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

	# Plugins are loaded at runtime and not linked at buildtime, so they should go to the same
	# output directory as the executables, so the plugin loader can find them there.
	set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

	# Remove default library prefix (e.g. "lib" on Linux), as the plugin loader doesn't know about it
	set_target_properties(${PROJECTNAME} PROPERTIES PREFIX "")

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
		if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "Intel")
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
		endif ()
	endif (MYGUI_STATIC)
	mygui_install_target(${PROJECTNAME} "")

	if (MYGUI_INSTALL_PDB)
		# install debug pdb files
		if (MYGUI_STATIC)
			install(FILES ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_DEBUG_PATH}/${PROJECTNAME}Static_d.pdb
				DESTINATION lib${MYGUI_LIB_DEBUG_PATH}
				CONFIGURATIONS Debug
			)
			install(FILES ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}${MYGUI_LIB_RELWDBG_PATH}/${PROJECTNAME}Static.pdb
				DESTINATION lib${MYGUI_LIB_RELWDBG_PATH}
				CONFIGURATIONS RelWithDebInfo
			)
		else ()
			install(FILES ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}${MYGUI_DEBUG_PATH}/${PROJECTNAME}_d.pdb
				DESTINATION bin${MYGUI_DEBUG_PATH}
				CONFIGURATIONS Debug
			)
			install(FILES ${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}${MYGUI_RELWDBG_PATH}/${PROJECTNAME}.pdb
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
			INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
		set_property(TARGET ${PROJECTNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
	endif ()
endfunction(mygui_config_sample)
