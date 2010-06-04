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

if (WIN32)
	add_definitions(-DNOMINMAX)
endif ()

# setup MyGUIEngine target
add_library(${PROJECTNAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(${PROJECTNAME} PROPERTIES VERSION ${MYGUI_VERSION} SOVERSION ${MYGUI_VERSION_MAJOR})
if (MYGUI_USE_FREETYPE)
	target_link_libraries(${PROJECTNAME}
		${FREETYPE_LIBRARIES}
	)
endif()

# platform specific dependencies
if (APPLE)
	include_directories(/usr/local/include /usr/include)
	target_link_libraries(${PROJECTNAME} ${Carbon_LIBRARY})
endif ()

# specify a precompiled header to use
use_precompiled_header(${PROJECTNAME}
  "${CMAKE_CURRENT_SOURCE_DIR}/include/MyGUI_Precompiled.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/MyGUI_Precompiled.cpp"
)

set(${PROJECTNAME}_HEADERS ${HEADER_FILES}) # for Mac OS X
mygui_config_lib(${PROJECTNAME})

# link Carbon on Mac OS X
if (APPLE)
	find_library(CARBON_LIBRARY Carbon)
	target_link_libraries(${PROJECTNAME} ${CARBON_LIBRARY})
endif ()

# install MyGUIEngine headers
install(FILES ${HEADER_FILES}
  DESTINATION include/MYGUI
)
