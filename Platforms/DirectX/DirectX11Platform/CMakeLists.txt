set (PROJECTNAME MyGUI.DirectX11Platform)

include_directories(
 include
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 ${DirectX_INCLUDE_DIR}
 ${MYGUI_SOURCE_DIR}/Common
)

include(${PROJECTNAME}.list)

add_library(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(${PROJECTNAME} MyGUIEngine)

target_link_libraries(${PROJECTNAME} ${DirectX11_LIBRARIES})
link_directories(${DirectX_LIBRARY_DIRS})
