include(PrecompiledHeader)

include_directories(
 include
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 ${OGRE_INCLUDE_DIR}
)

include(MyGUI.DirectXPlatform.list)

add_definitions("-DMYGUI_DIRECTX_PLATFORM")

# setup MyGUIEngine target
add_library(MyGUI.DirectXPlatform ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(MyGUI.DirectXPlatform MyGUIEngine)

target_link_libraries(MyGUI.DirectXPlatform ${DirectX_LIBRARIES})
link_directories(${DirectX_LIBRARY_DIRS})
