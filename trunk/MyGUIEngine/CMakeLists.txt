set (PROJECTNAME MyGUIEngine)

project( ${PROJECTNAME} )

include(PrecompiledHeader)

include(${PROJECTNAME}.list)

include_directories(include)
if (MYGUI_USE_FREETYPE)
	include_directories(
		${FREETYPE_INCLUDE_DIRS}
	)
	add_definitions(-DMYGUI_USE_FREETYPE)
endif ()

if (MYGUI_DONT_USE_OBSOLETE)
	add_definitions(-DMYGUI_DONT_USE_OBSOLETE)
endif ()

add_definitions(-DMYGUI_BUILD)

# setup MyGUIEngine target
add_library(${PROJECTNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(${PROJECTNAME} PROPERTIES VERSION ${MYGUI_VERSION} SOVERSION ${MYGUI_VERSION_MAJOR})
if (MYGUI_USE_FREETYPE)
	target_link_libraries(${PROJECTNAME}
		${FREETYPE_LIBRARIES}
	)
endif()

# specify a precompiled header to use
use_precompiled_header(${PROJECTNAME}
  "${CMAKE_CURRENT_SOURCE_DIR}/include/MyGUI_Precompiled.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/MyGUI_Precompiled.cpp"
)

mygui_config_lib(${PROJECTNAME})

# install MyGUIEngine headers
install(FILES ${HEADER_FILES}
  DESTINATION include/MYGUI
)
