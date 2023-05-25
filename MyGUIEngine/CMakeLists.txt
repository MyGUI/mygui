set (PROJECTNAME MyGUIEngine)

project( ${PROJECTNAME} )

include(${PROJECTNAME}.list)

include_directories(include)
if (MYGUI_USE_FREETYPE)
	include_directories(SYSTEM
		${FREETYPE_INCLUDE_DIRS}
	)
	add_definitions(-DMYGUI_USE_FREETYPE)

	if (MYGUI_MSDF_FONTS)
		add_definitions(-DMYGUI_MSDF_FONTS)
	endif ()
endif ()
if (MYGUI_DISABLE_PLUGINS)
	add_definitions(-DMYGUI_DISABLE_PLUGINS)
endif ()

add_definitions(-DMYGUI_BUILD)

# setup MyGUIEngine target
add_library(${PROJECTNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
if (NOT ANDROID)
	set_target_properties(${PROJECTNAME} PROPERTIES VERSION ${MYGUI_VERSION} SOVERSION "${MYGUI_VERSION_MAJOR}.${MYGUI_VERSION_MINOR}.${MYGUI_VERSION_PATCH}")
endif()

if (UNIX)
	# some platforms require explicit linking to libdl, see if it's available
	find_library(DL_LIBRARY NAMES dl dlsym)
	mark_as_advanced(DL_LIBRARY)
	if (DL_LIBRARY)
		target_link_libraries(${PROJECTNAME} dl)
	endif ()
endif()

if (MYGUI_USE_FREETYPE)
	if (MYGUI_MSDF_FONTS)
		add_subdirectory(src/msdfgen)
		target_link_libraries(${PROJECTNAME} msdfgen)
	endif ()

	target_link_libraries(${PROJECTNAME} ${FREETYPE_LIBRARIES})
	if (ZLIB_FOUND) # hacky way to check if freetype was built with zlib
		target_link_libraries(${PROJECTNAME} ${ZLIB_LIBRARIES})
	endif()
endif()

if (${CMAKE_VERSION} VERSION_EQUAL "3.16" OR ${CMAKE_VERSION} VERSION_GREATER "3.16")
	if (NOT MYGUI_CLANG_TIDY_BUILD)
		target_precompile_headers(${PROJECTNAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/MyGUI_Precompiled.h")
	else ()
		target_compile_options(${PROJECTNAME} PRIVATE -include "${CMAKE_CURRENT_SOURCE_DIR}/include/MyGUI_Precompiled.h")
	endif ()
endif ()

set(${PROJECTNAME}_HEADERS ${HEADER_FILES}) # for Mac OS X
mygui_config_lib(${PROJECTNAME})

# install MyGUIEngine headers
install(FILES ${HEADER_FILES}
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
)
