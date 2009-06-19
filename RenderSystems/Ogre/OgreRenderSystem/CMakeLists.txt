include(PrecompiledHeader)

include_directories(
 include
 ${MYGUI_SOURCE_DIR}/Common
 ${MYGUI_SOURCE_DIR}/Common/Base
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 ${OGRE_INCLUDE_DIR}
)

include(OgreRenderSystem.list)

add_definitions("-DBASE_PLATFORM_OGRE")

# setup MyGUIEngine target
add_library(OgreRenderSystem ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(OgreRenderSystem MyGUIEngine)

