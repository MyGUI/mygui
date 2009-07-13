include(PrecompiledHeader)

include_directories(
 include
 ${MYGUI_SOURCE_DIR}/Common
 ${MYGUI_SOURCE_DIR}/Common/Base
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 ${OGRE_INCLUDE_DIR}
)

include(MyGUI.OpenGLPlatform.list)

add_definitions("-DMYGUI_OPENGL_PLATFORM")

# setup MyGUIEngine target
add_library(MyGUI.OpenGLPlatform ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(MyGUI.OpenGLPlatform MyGUIEngine)

