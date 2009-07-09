set (PROJECT_NAME MyGUIEngine)

project( ${PROJECT_NAME} )

include(PrecompiledHeader)

include(${PROJECT_NAME}.list)

include_directories(include)
add_definitions(-DMYGUI_BUILD)

link_directories(${OGRE_LIB_DIR} ${OIS_LIB_DIR})

GET_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME}
  "include/MyGUI_Precompiled.h"
)

# setup MyGUIEngine target
add_library(${PROJECT_NAME} ${MYGUI_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES} ${${PROJECT_NAME}_pch})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${MYGUI_VERSION} SOVERSION ${MYGUI_VERSION_MAJOR})
target_link_libraries(${PROJECT_NAME}
  ${FREETYPE_LIBRARIES}
  ${OGRE_LIBRARIES}
)

# specify a precompiled header to use
ADD_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME}
  "include/MyGUI_Precompiled.h"
)
#USE_PRECOMPILED_HEADER(
#  ${PROJECT_NAME}
#  "include/MyGUI_Precompiled.h"
#  ${SOURCE_FILES}
#)

# install MyGUIEngine
mygui_config_lib(${PROJECT_NAME})
install(FILES ${HEADER_FILES}
  DESTINATION include/MyGUI
)
