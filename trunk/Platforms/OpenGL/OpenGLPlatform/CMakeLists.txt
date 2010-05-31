include_directories(
 include
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 ${OPENGL_INCLUDE_DIR}
 ${PNG_INCLUDE_DIR}
 ${ZLIB_INCLUDE_DIR}
 ${MYGUI_SOURCE_DIR}/Common
)

include(MyGUI.OpenGLPlatform.list)

add_library(MyGUI.OpenGLPlatform ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(MyGUI.OpenGLPlatform MyGUIEngine)

target_link_libraries(MyGUI.OpenGLPlatform ${OPENGL_LIBRARIES} ${PNG_LIBRARIES} ${ZLIB_LIBRARIES})
link_directories(${OPENGL_LIB_DIR} ${PNG_LIBRARY})
