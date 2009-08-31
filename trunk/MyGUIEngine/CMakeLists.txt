set (PROJECT_NAME MyGUIEngine)

project( ${PROJECT_NAME} )

include(PrecompiledHeader)

include(${PROJECT_NAME}.list)

include_directories(include)
add_definitions(-DMYGUI_BUILD)

# setup MyGUIEngine target
add_library(${PROJECT_NAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${MYGUI_VERSION} SOVERSION ${MYGUI_VERSION_MAJOR})
target_link_libraries(${PROJECT_NAME}
  ${FREETYPE_LIBRARIES}
)

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
