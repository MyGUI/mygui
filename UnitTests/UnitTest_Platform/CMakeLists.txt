mygui_unit_test(UnitTest_Platform GROUPED)
target_link_libraries(UnitTest_Platform PRIVATE MyGUICommon)
foreach(group resources rendering lifecycle)
	add_test(NAME UnitTest_Platform_${group}
		COMMAND UnitTest_Platform --group ${group}
			--artifacts "${CMAKE_CURRENT_BINARY_DIR}/artifacts/${group}")
	set_tests_properties(UnitTest_Platform_${group} PROPERTIES
		LABELS "platform;graphics" RUN_SERIAL TRUE TIMEOUT 120
		WORKING_DIRECTORY "$<TARGET_FILE_DIR:UnitTest_Platform>")
	if(MYGUI_RENDERSYSTEM EQUAL 11)
		# OSG may log and consume an error before the frame-capture check sees it.
		set_tests_properties(UnitTest_Platform_${group} PROPERTIES
			ENVIRONMENT "OSG_NOTIFY_LEVEL=NOTICE"
			FAIL_REGULAR_EXPRESSION "Warning: detected OpenGL error")
	endif()
endforeach()
