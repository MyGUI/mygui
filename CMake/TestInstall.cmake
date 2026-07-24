# Module to test installed MyGUI targets.
# Used in the Scripts/test_install.py
# Requires MYGUI_RENDERSYSTEM to be set (e.g., -DMYGUI_RENDERSYSTEM=7).

include(Utils/MyGUIConfigTargets)
mygui_set_platform_name(${MYGUI_RENDERSYSTEM})

find_package(MyGUI REQUIRED)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/TestInstall.cpp" [[
#include <MyGUI.h>
#include <MyGUI_ResourceTrueTypeFont.h>
int main() {
    MyGUI::Gui gui;
    gui.initialise("");
    gui.shutdown();
    return 0;
}
]])

add_executable(TestInstall "${CMAKE_CURRENT_BINARY_DIR}/TestInstall.cpp")
target_link_libraries(TestInstall PRIVATE MyGUI::MyGUI MyGUI::${MYGUI_PLATFORM_NAME}Platform)
