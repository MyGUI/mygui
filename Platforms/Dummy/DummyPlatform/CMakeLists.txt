set (PROJECTNAME MyGUI.DummyPlatform)

include_directories(
 include
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 #${DirectX_INCLUDE_DIR}
 ${MYGUI_SOURCE_DIR}/Common
)

include(${PROJECTNAME}.list)

add_library(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(${PROJECTNAME} MyGUIEngine)

target_link_libraries(${PROJECTNAME} ${DUMMY_LIBRARIES})
link_directories(${DUMMY_LIBRARY_DIRS})
