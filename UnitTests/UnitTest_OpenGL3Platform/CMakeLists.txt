mygui_unit_test(UnitTest_OpenGL3Platform)
target_link_libraries(UnitTest_OpenGL3Platform PRIVATE MyGUICommon)
target_include_directories(UnitTest_OpenGL3Platform PRIVATE ../UnitTest_Platform)
set_tests_properties(UnitTest_OpenGL3Platform PROPERTIES
	LABELS "platform;graphics" RUN_SERIAL TRUE TIMEOUT 120
	WORKING_DIRECTORY "$<TARGET_FILE_DIR:UnitTest_OpenGL3Platform>")
