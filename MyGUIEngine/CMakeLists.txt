set (PROJECTNAME MyGUIEngine)

project( ${PROJECTNAME} )

include(${PROJECTNAME}.list)

add_library(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECTNAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/MYGUI>
)

target_compile_definitions(${PROJECTNAME}
	PUBLIC
		$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:MYGUI_STATIC>
	PRIVATE
		MYGUI_BUILD
		$<$<BOOL:${MYGUI_USE_FREETYPE}>:MYGUI_USE_FREETYPE>
		$<$<BOOL:${MYGUI_MSDF_FONTS}>:MYGUI_MSDF_FONTS>
		$<$<BOOL:${MYGUI_DISABLE_PLUGINS}>:MYGUI_DISABLE_PLUGINS>
)

if (NOT ANDROID)
	set_target_properties(${PROJECTNAME} PROPERTIES VERSION ${MYGUI_VERSION} SOVERSION "${MYGUI_VERSION_MAJOR}.${MYGUI_VERSION_MINOR}.${MYGUI_VERSION_PATCH}")
endif()

if (MYGUI_USE_FREETYPE)
	target_link_libraries(${PROJECTNAME} PRIVATE Freetype::Freetype)

	if (MYGUI_MSDF_FONTS)
		if (MYGUI_USE_SYSTEM_MSDFGEN)
			find_package(msdfgen)
		else()
			add_subdirectory(src/msdfgen)
		endif()
		target_link_libraries(${PROJECTNAME} PRIVATE msdfgen::msdfgen)
	endif()
endif()

if (NOT MYGUI_CLANG_TIDY_BUILD)
    target_precompile_headers(${PROJECTNAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/MyGUI_Precompiled.h")
else ()
    target_compile_options(${PROJECTNAME} PRIVATE -include "${CMAKE_CURRENT_SOURCE_DIR}/include/MyGUI_Precompiled.h")
endif ()

set(${PROJECTNAME}_HEADERS ${HEADER_FILES}) # for Mac OS X
mygui_config_lib(${PROJECTNAME})
mygui_install_target(${PROJECTNAME})

# install MyGUIEngine headers
install(FILES ${HEADER_FILES}
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
)

set_target_properties(MyGUIEngine PROPERTIES EXPORT_NAME MyGUI)
add_library(MyGUI::MyGUI ALIAS MyGUIEngine)
