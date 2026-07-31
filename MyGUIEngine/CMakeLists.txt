set(PROJECTNAME MyGUIEngine)

project(${PROJECTNAME})

include(${PROJECTNAME}.list)

# Generate MyGUI_Config.h from template so non-CMake users have the correct defines
set(MYGUI_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
set(MYGUI_STATIC OFF)
if(NOT BUILD_SHARED_LIBS)
	set(MYGUI_STATIC ON)
endif()
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/include/MyGUI_Config.h.in"
	"${MYGUI_CONFIG_DIR}/MyGUI_Config.h"
	@ONLY
)

add_library(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECTNAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${MYGUI_CONFIG_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/MYGUI>
)

target_compile_definitions(${PROJECTNAME}
	PRIVATE
		MYGUI_BUILD
)

if(NOT ANDROID)
	set_target_properties(${PROJECTNAME} PROPERTIES
		VERSION ${MYGUI_VERSION}
		SOVERSION
			"${MYGUI_VERSION_MAJOR}.${MYGUI_VERSION_MINOR}.${MYGUI_VERSION_PATCH}"
	)
endif()

if(MYGUI_USE_FREETYPE)
	target_link_libraries(${PROJECTNAME} PRIVATE Freetype::Freetype)

	if(MYGUI_MSDF_FONTS)
		target_link_libraries(${PROJECTNAME} PRIVATE msdfgen::msdfgen)
	endif()
endif()

target_link_libraries(${PROJECTNAME} PRIVATE pugixml::pugixml)

mygui_target_precompile_headers(${PROJECTNAME} "${CMAKE_CURRENT_SOURCE_DIR}/include/MyGUI_Precompiled.h")

set(${PROJECTNAME}_HEADERS ${HEADER_FILES}) # for Mac OS X
mygui_config_lib(${PROJECTNAME})
mygui_install_target(${PROJECTNAME})

# install MyGUIEngine headers
install(FILES ${HEADER_FILES}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
)

# install generated configuration header
install(FILES "${MYGUI_CONFIG_DIR}/MyGUI_Config.h"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
)

set_target_properties(MyGUIEngine PROPERTIES EXPORT_NAME MyGUI)
add_library(MyGUI::MyGUI ALIAS MyGUIEngine)
