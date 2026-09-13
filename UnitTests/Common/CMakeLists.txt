add_library(MyGUIUnitTestCommon STATIC TestSupport.h TestSupport.cpp)
set_target_properties(MyGUIUnitTestCommon PROPERTIES FOLDER UnitTest)
target_include_directories(MyGUIUnitTestCommon PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${MYGUI_SOURCE_DIR}/Platforms/DummyPlatform/include"
)
target_link_libraries(MyGUIUnitTestCommon PUBLIC MyGUI::MyGUI)
if(EMSCRIPTEN)
	target_compile_definitions(MyGUIUnitTestCommon PRIVATE MYGUI_UNITTEST_RESOURCE_ROOT="/UnitTests")
	target_link_options(MyGUIUnitTestCommon INTERFACE
		"SHELL:--preload-file ${MYGUI_SOURCE_DIR}/Media/UnitTests@/UnitTests"
	)
else()
	target_compile_definitions(MyGUIUnitTestCommon PRIVATE
		MYGUI_UNITTEST_RESOURCE_ROOT="${MYGUI_SOURCE_DIR}/Media/UnitTests"
	)
endif()
if(TARGET MyGUI.DummyPlatform)
	target_link_libraries(MyGUIUnitTestCommon PUBLIC MyGUI.DummyPlatform)
else()
	target_sources(MyGUIUnitTestCommon PRIVATE
		"${MYGUI_SOURCE_DIR}/Platforms/DummyPlatform/src/MyGUI_DummyDataManager.cpp"
		"${MYGUI_SOURCE_DIR}/Platforms/DummyPlatform/src/MyGUI_DummyRenderManager.cpp"
	)
endif()
