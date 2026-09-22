add_library(MyGUIUnitTestCommon STATIC
		TestSupport.h
		TestRunner.h
		TestSupport.cpp
		BehaviourTestSupport.h
		FixedFont.h
		SkinTestContext.h
)
set_target_properties(MyGUIUnitTestCommon PROPERTIES FOLDER UnitTest)
target_include_directories(MyGUIUnitTestCommon PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${MYGUI_SOURCE_DIR}/Platforms/DummyPlatform/include"
)
target_link_libraries(MyGUIUnitTestCommon PUBLIC MyGUI::MyGUI)
add_library(MyGUISkinTestSupport INTERFACE)
target_link_libraries(MyGUISkinTestSupport INTERFACE MyGUIUnitTestCommon)
if(EMSCRIPTEN)
	target_link_options(MyGUISkinTestSupport INTERFACE
			"SHELL:--preload-file ${MYGUI_SOURCE_DIR}/Media/MyGUI_Media@/MyGUI_Media"
	)
	target_compile_definitions(MyGUIUnitTestCommon PRIVATE
		MYGUI_UNITTEST_RESOURCE_ROOT="/UnitTests"
		MYGUI_UNITTEST_MEDIA_ROOT="/MyGUI_Media"
	)
	target_link_options(MyGUIUnitTestCommon INTERFACE
		"SHELL:--preload-file ${MYGUI_SOURCE_DIR}/Media/UnitTests@/UnitTests"
	)
	mygui_emscripten_exclude_non_runtime_media(MyGUIUnitTestCommon INTERFACE)
else()
	target_compile_definitions(MyGUIUnitTestCommon PRIVATE
		MYGUI_UNITTEST_RESOURCE_ROOT="${MYGUI_SOURCE_DIR}/Media/UnitTests"
		MYGUI_UNITTEST_MEDIA_ROOT="${MYGUI_SOURCE_DIR}/Media/MyGUI_Media"
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
