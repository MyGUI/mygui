mygui_unit_test(UnitTest_Layers)
if(EMSCRIPTEN)
	target_compile_definitions(UnitTest_Layers PRIVATE MYGUI_UNITTEST_RESOURCE_ROOT="/UnitTests")
else()
	target_compile_definitions(UnitTest_Layers PRIVATE
		MYGUI_UNITTEST_RESOURCE_ROOT="${MYGUI_SOURCE_DIR}/Media/UnitTests")
endif()
