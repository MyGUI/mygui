

project( MyGUIEngine )

include(PrecompiledHeader)

file(GLOB HEADER_FILES include/*.h)
file(GLOB SOURCE_FILES src/*.cpp)

include(MyGUIEngine.list)

add_definitions(-DMYGUI_BUILD)

# setup MyGUIEngine target
add_library(MyGUIEngine ${MYGUI_LIB_TYPE} ${PREC_HEADER} ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(MyGUIEngine PROPERTIES VERSION ${MYGUI_VERSION} SOVERSION ${MYGUI_VERSION_MAJOR})
target_link_libraries(MyGUIEngine
  ${FREETYPE_LIBRARIES}
  ${OGRE_LIBRARIES}
)

# specify a precompiled header to use
use_precompiled_header(MyGUIEngine 
  "${CMAKE_CURRENT_SOURCE_DIR}/include/MyGUI_Precompiled.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/MyGUI_Precompiled.cpp"
)

# install MyGUIEngine
mygui_config_lib(MyGUIEngine)
install(FILES ${HEADER_FILES}
  DESTINATION include/MyGUI
)
