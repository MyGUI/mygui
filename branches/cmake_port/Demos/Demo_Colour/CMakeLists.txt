
include_directories(
 .
 ${MYGUI_SOURCE_DIR}/Common
 ${MYGUI_SOURCE_DIR}/Common/Base
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 ${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/include
)

file(GLOB demofiles *.h *.cpp)
add_definitions("-DBASE_PLATFORM_OGRE")

set(demofiles ${demofiles}
 ${MYGUI_SOURCE_DIR}/Common/Base/Main.cpp
 ${MYGUI_SOURCE_DIR}/Common/Base/BaseManager.cpp
 ${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/src/MyGUI_OgreRenderManager.cpp
 ${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/src/MyGUI_OgreTexture.cpp
 ${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/src/MyGUI_OgreVertexBuffer.cpp
 ${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/src/MyGUI_OgreDataManager.cpp
 ${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/src/MyGUI_OgreViewport.cpp
)

# setup MyGUIEngine target
add_executable(demo_colour ${demofiles})

add_dependencies(demo_colour OgreMain MyGUIEngine)

target_link_libraries(demo_colour
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 uuid
)


