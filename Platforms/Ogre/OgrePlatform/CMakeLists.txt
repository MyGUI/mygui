include(PrecompiledHeader)

include_directories(
 include
 ${MYGUI_SOURCE_DIR}/Common
 ${MYGUI_SOURCE_DIR}/Common/Base
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 ${OGRE_INCLUDE_DIR}
)

include(MyGUI.OgrePlatform.list)

add_definitions("-DBASE_PLATFORM_OGRE")

# setup MyGUIEngine target
add_library(MyGUI.OgrePlatform ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(MyGUI.OgrePlatform MyGUIEngine)

