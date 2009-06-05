

project( MyGUIEngine )

include(PrecompiledHeader)

# define header and source files for the library
file(GLOB HEADER_FILES include/*.h)

file(GLOB SOURCE_FILES src/*.cpp)

include(MyGUIEngine.list)

# setup MyGUIEngine target
add_library(MyGUIEngine ${MYGUI_LIB_TYPE} ${PREC_HEADER} ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(MyGUIEngine PROPERTIES VERSION ${MYGUI_VERSION} SOVERSION ${MYGUI_VERSION_MAJOR})
if (APPLE)
  set_target_properties(MyGUIEngine PROPERTIES	FRAMEWORK TRUE)
  set_target_properties(MyGUIEngine PROPERTIES PUBLIC_HEADER "${HEADER_FILES}" )
  # Framework is called 'Ogre'
  set_target_properties(MyGUIEngine PROPERTIES	OUTPUT_NAME MyGUI)
  # Set the INSTALL_PATH so that Frameworks can be local
  set_target_properties(MyGUIEngine
     PROPERTIES BUILD_WITH_INSTALL_RPATH 1
     INSTALL_NAME_DIR "@executable_path/../Frameworks"
  )
endif()
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