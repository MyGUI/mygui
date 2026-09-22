mygui_unit_test(UnitTest_Platform GROUPED)
target_link_libraries(UnitTest_Platform PRIVATE MyGUICommon)
if (MYGUI_RENDERSYSTEM EQUAL 10)
	find_program(MYGUI_PLATFORM_TEST_GLSLANG NAMES glslangValidator
			HINTS "$ENV{VULKAN_SDK}/bin" REQUIRED)
	# Match the executable directory without target-dependent expressions in OUTPUT.
	get_target_property(shader_dir UnitTest_Platform RUNTIME_OUTPUT_DIRECTORY)
	if (CMAKE_CONFIGURATION_TYPES)
		string(APPEND shader_dir "/$<CONFIG>")
	endif ()
	string(APPEND shader_dir "/GeneratedShaders")
	add_custom_command(OUTPUT "${shader_dir}/PlatformSwap_Vulkan_FP.spv"
			COMMAND ${CMAKE_COMMAND} -E make_directory "${shader_dir}"
			COMMAND "${MYGUI_PLATFORM_TEST_GLSLANG}" -V
			"${MYGUI_SOURCE_DIR}/Media/UnitTests/UnitTest_Platform/PlatformSwap_Vulkan_FP.frag"
			-o "${shader_dir}/PlatformSwap_Vulkan_FP.spv"
			DEPENDS "${MYGUI_SOURCE_DIR}/Media/UnitTests/UnitTest_Platform/PlatformSwap_Vulkan_FP.frag"
			VERBATIM)
	add_custom_target(UnitTest_Platform_Shaders
			DEPENDS "${shader_dir}/PlatformSwap_Vulkan_FP.spv")
	add_dependencies(UnitTest_Platform UnitTest_Platform_Shaders)
endif ()
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
