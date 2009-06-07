
include_directories(
 .
 ${MYGUI_SOURCE_DIR}/Common
 ${MYGUI_SOURCE_DIR}/Common/Base
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 ${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/include
)

file(GLOB demofiles *.h *.cpp)
add_definitions("-DBASE_PLATFORM_OGRE")

# setup MyGUIEngine target
add_executable(demo_colour ${demofiles})

add_dependencies(demo_colour MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

target_link_libraries(demo_colour
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)


