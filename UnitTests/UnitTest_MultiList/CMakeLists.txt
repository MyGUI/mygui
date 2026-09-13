mygui_unit_test(UnitTest_MultiList)

# Exercise list operations without a window or graphics context.
set_target_properties(UnitTest_MultiList PROPERTIES WIN32_EXECUTABLE FALSE)
target_include_directories(UnitTest_MultiList PRIVATE "${MYGUI_SOURCE_DIR}/Platforms/DummyPlatform/include")
if(TARGET MyGUI.DummyPlatform)
	target_link_libraries(UnitTest_MultiList PRIVATE MyGUI.DummyPlatform)
else()
	target_sources(UnitTest_MultiList PRIVATE
		"${MYGUI_SOURCE_DIR}/Platforms/DummyPlatform/src/MyGUI_DummyDataManager.cpp"
		"${MYGUI_SOURCE_DIR}/Platforms/DummyPlatform/src/MyGUI_DummyRenderManager.cpp"
	)
endif()
