# Module to test installed MyGUI targets.
# Used in Scripts/Tests/test_install.py
# Requires MYGUI_RENDERSYSTEM to be set (e.g., -DMYGUI_RENDERSYSTEM=7).

include(Utils/MyGUIConfigTargets)
if(NOT DEFINED MYGUI_RENDERSYSTEM)
	message(FATAL_ERROR "Set MYGUI_RENDERSYSTEM to the installed backend ID")
endif()
mygui_set_platform_name(${MYGUI_RENDERSYSTEM})

find_package(MyGUI REQUIRED)

set(_consumer_source [[
#include <MyGUI.h>
#include <MyGUI_@MYGUI_PLATFORM_NAME@Platform.h>
#include <MyGUI_ResourceTrueTypeFont.h>
int main() {
    // Construction pulls the backend implementation and its dependencies into
    // the executable without requiring a window or graphics device.
    MyGUI::@MYGUI_PLATFORM_NAME@Platform platform;
    if (!platform.getRenderManagerPtr() || !platform.getDataManagerPtr())
        return 1;
    return 0;
}
]])
string(CONFIGURE "${_consumer_source}" _consumer_source @ONLY)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/TestInstall.cpp" "${_consumer_source}")

add_executable(TestInstall "${CMAKE_CURRENT_BINARY_DIR}/TestInstall.cpp")
target_link_libraries(TestInstall PRIVATE MyGUI::MyGUI MyGUI::${MYGUI_PLATFORM_NAME}Platform)

if(WIN32)
	file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/copy-consumer-dlls-$<CONFIG>.cmake"
		CONTENT "set(dlls \"$<TARGET_RUNTIME_DLLS:TestInstall>\")\nif(dlls)\n  file(COPY \${dlls} DESTINATION \"$<TARGET_FILE_DIR:TestInstall>\")\nendif()\n"
	)
	add_custom_command(TARGET TestInstall POST_BUILD
		COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/copy-consumer-dlls-$<CONFIG>.cmake"
		VERBATIM
	)
endif()
enable_testing()
add_test(NAME InstalledPlatform COMMAND TestInstall)
set_tests_properties(InstalledPlatform PROPERTIES TIMEOUT 30)
