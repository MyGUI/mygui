mygui_unit_test(UnitTest_Platform GROUPED)
target_link_libraries(UnitTest_Platform PRIVATE MyGUICommon)
if(EMSCRIPTEN)
	find_package(Python3 REQUIRED COMPONENTS Interpreter)
	set(_web_shell "${CMAKE_CURRENT_SOURCE_DIR}/WebShell.html")
	set_target_properties(UnitTest_Platform PROPERTIES SUFFIX ".html")
	target_link_options(UnitTest_Platform PRIVATE
		"SHELL:--shell-file \"${_web_shell}\""
		"-sEXIT_RUNTIME=1"
		"-sEXPORTED_RUNTIME_METHODS=FS")
	set_property(TARGET UnitTest_Platform APPEND PROPERTY LINK_DEPENDS "${_web_shell}")
endif()
foreach(group resources rendering lifecycle)
	if(EMSCRIPTEN)
		add_test(NAME UnitTest_Platform_${group}
			COMMAND "${Python3_EXECUTABLE}" "${MYGUI_SOURCE_DIR}/Scripts/Tests/run_browser_test.py"
				"$<TARGET_FILE:UnitTest_Platform>"
				--artifacts "${CMAKE_CURRENT_BINARY_DIR}/artifacts/${group}"
				-- --group ${group} --artifacts /artifacts --visible)
	else()
		add_test(NAME UnitTest_Platform_${group}
			COMMAND UnitTest_Platform --group ${group}
				--artifacts "${CMAKE_CURRENT_BINARY_DIR}/artifacts/${group}")
	endif()
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
