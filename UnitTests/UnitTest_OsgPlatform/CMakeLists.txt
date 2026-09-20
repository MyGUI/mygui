mygui_unit_test(UnitTest_OsgPlatform)
target_compile_definitions(UnitTest_OsgPlatform PRIVATE
	MYGUI_OSG_TEST_MEDIA="${MYGUI_SOURCE_DIR}/Media/MyGUI_Media"
	MYGUI_OSG_TEST_RESOURCES="${MYGUI_SOURCE_DIR}/Media/UnitTests/UnitTest_Platform"
)
if(TARGET MyGUICommon AND MYGUI_RENDERSYSTEM EQUAL 11)
	target_sources(UnitTest_OsgPlatform PRIVATE Render.cpp)
	target_link_libraries(UnitTest_OsgPlatform PRIVATE MyGUICommon)
	target_compile_definitions(UnitTest_OsgPlatform PRIVATE MYGUI_OSG_RENDER_TESTS)
	add_test(NAME UnitTest_OsgRendering COMMAND UnitTest_OsgPlatform --render)
	set_tests_properties(UnitTest_OsgRendering PROPERTIES LABELS "graphics" RUN_SERIAL TRUE)
else()
	target_link_libraries(UnitTest_OsgPlatform PRIVATE MyGUI.OsgPlatform)
endif()
