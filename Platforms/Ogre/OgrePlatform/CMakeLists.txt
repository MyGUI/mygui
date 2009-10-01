include_directories(
 include
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 ${OGRE_INCLUDE_DIR}
)

include(MyGUI.OgrePlatform.list)

# setup MyGUIEngine target
add_library(MyGUI.OgrePlatform ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(MyGUI.OgrePlatform MyGUIEngine)

target_link_libraries(MyGUI.OgrePlatform ${OGRE_LIBRARIES})
link_directories(${OGRE_LIB_DIR})
