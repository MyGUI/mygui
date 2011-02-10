include_directories(${BERKELIUM_INCLUDE_DIRS})
include_directories(${MYGUI_SOURCE_DIR}/Plugins/Plugin_BerkeliumWidget)
mygui_demo(Demo_PluginBerkeliumWidget)
add_dependencies(Demo_PluginBerkeliumWidget Plugin_BerkeliumWidget)