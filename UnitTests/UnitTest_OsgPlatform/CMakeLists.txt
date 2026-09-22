mygui_unit_test(UnitTest_OsgPlatform)
target_link_libraries(UnitTest_OsgPlatform PRIVATE MyGUICommon)
set_tests_properties(UnitTest_OsgPlatform PROPERTIES
	LABELS "platform;graphics" RUN_SERIAL TRUE TIMEOUT 120
	ENVIRONMENT "OSG_NOTIFY_LEVEL=NOTICE"
	FAIL_REGULAR_EXPRESSION "Warning: detected OpenGL error")
