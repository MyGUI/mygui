get_property(_demos GLOBAL PROPERTY MYGUI_WEB_DEMO_TARGETS)
if(NOT _demos)
	message(FATAL_ERROR "MYGUI_BUILD_WEB_DEMOS requires standard or advanced demos")
endif()

set(MYGUI_WEB_DEMO_DECLARATIONS "")
set(MYGUI_WEB_DEMO_ENTRIES "")
set(MYGUI_WEB_DEMO_NAMES "[")
foreach(_demo IN LISTS _demos)
	string(APPEND MYGUI_WEB_DEMO_DECLARATIONS "int mygui_start_${_demo}(int argc, char** argv);\n")
	string(APPEND MYGUI_WEB_DEMO_ENTRIES "\t\t{\"${_demo}\", mygui_start_${_demo}},\n")
	string(APPEND MYGUI_WEB_DEMO_NAMES "\"${_demo}\",")
endforeach()
string(APPEND MYGUI_WEB_DEMO_NAMES "]")

set(_generated "${MYGUI_BINARY_DIR}/web-demos")
configure_file("${MYGUI_SOURCE_DIR}/Common/WebDemos.cpp.in" "${_generated}/WebDemos.cpp" @ONLY)
configure_file("${MYGUI_SOURCE_DIR}/Common/WebDemos.js.in" "${_generated}/WebDemos.js" @ONLY)
add_executable(MyGUI_Demos "${_generated}/WebDemos.cpp")
mygui_config_common(MyGUI_Demos)
target_link_libraries(MyGUI_Demos PRIVATE MyGUICommon)
foreach(_demo IN LISTS _demos)
	target_link_libraries(MyGUI_Demos PRIVATE ${_demo}_web)
endforeach()
target_link_options(MyGUI_Demos PRIVATE
	"SHELL:--shell-file \"${MYGUI_SOURCE_DIR}/Common/WebShell.html\""
	"SHELL:--pre-js \"${_generated}/WebDemos.js\""
)
set_property(TARGET MyGUI_Demos APPEND PROPERTY LINK_DEPENDS
	"${MYGUI_SOURCE_DIR}/Common/WebShell.html"
	"${_generated}/WebDemos.js"
)
