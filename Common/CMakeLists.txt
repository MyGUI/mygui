set (PROJECT_NAME MyGUIFramework)

include_directories(
 .
 ${MYGUI_SOURCE_DIR}/Common
 ${MYGUI_SOURCE_DIR}/Common/Base
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 ${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/include
 ${OGRE_INCLUDE_DIR}
)

include(${PROJECT_NAME}.list)

add_definitions("-DBASE_PLATFORM_OGRE")

# setup MyGUIEngine target
add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(${PROJECT_NAME} MyGUIEngine)

