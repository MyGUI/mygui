include(PrecompiledHeader)

include_directories(
 include
 ${MYGUI_SOURCE_DIR}/Common
 ${MYGUI_SOURCE_DIR}/Common/Base
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 ${OGRE_INCLUDE_DIR}
)

include(MyGUI.DirectXPlatform.list)

add_definitions("-DMYGUI_DIRECTX_PLATFORM")

# setup MyGUIEngine target
add_library(MyGUI.DirectXPlatform ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(MyGUI.DirectXPlatform MyGUIEngine)

target_link_libraries(MyGUI.DirectXPlatform
  OgreMain
  d3d9
  d3dx9
  dxerr9
  dxguid
)
