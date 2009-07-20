include(PrecompiledHeader)

include_directories(
 include
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 ${OGRE_INCLUDE_DIR}
)

include(MyGUI.OgrePlatform.list)

link_directories(${OGRE_LIB_DIR})

# setup MyGUIEngine target
add_library(MyGUI.OgrePlatform ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(MyGUI.OgrePlatform MyGUIEngine)

target_link_libraries(MyGUI.OgrePlatform ${OGRE_LIBRARIES})