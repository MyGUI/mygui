set (PROJECT_NAME MyGUIEngine)

project( ${PROJECT_NAME} )

include(PrecompiledHeader)

include(${PROJECT_NAME}.list)

if (MYGUI_USE_FREETYPE)
	include_directories(include
		${FREETYPE_INCLUDE_DIRS}
	)
	add_definitions(-DMYGUI_USE_FREETYPE)
endif ()

if (MYGUI_NO_OIS)
	add_definitions(-DMYGUI_NO_OIS)
endif ()

if (MYGUI_DONT_USE_OBSOLETE)
	add_definitions(-DMYGUI_DONT_USE_OBSOLETE)
endif ()

add_definitions(-DMYGUI_BUILD)

# setup MyGUIEngine target
add_library(${PROJECT_NAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${MYGUI_VERSION} SOVERSION ${MYGUI_VERSION_MAJOR})
if (MYGUI_USE_FREETYPE)
	target_link_libraries(${PROJECT_NAME}
		${FREETYPE_LIBRARIES}
	)
endif()

# specify a precompiled header to use
use_precompiled_header(${PROJECT_NAME}
  "${CMAKE_CURRENT_SOURCE_DIR}/include/MyGUI_Precompiled.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/MyGUI_Precompiled.cpp"
)

# install MyGUIEngine
mygui_config_lib(${PROJECT_NAME})
install(FILES ${HEADER_FILES}
  DESTINATION include/MyGUI
)
