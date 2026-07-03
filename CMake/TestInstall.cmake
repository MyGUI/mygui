# Module to test installed MyGUI targets.
# Used in the Scripts/test_install.py

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
target_link_libraries(TestInstall PRIVATE MyGUI::MyGUI MyGUI::OpenGL3Platform)
