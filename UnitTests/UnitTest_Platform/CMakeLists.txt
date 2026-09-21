mygui_unit_test(UnitTest_Platform GROUPED)
target_link_libraries(UnitTest_Platform PRIVATE MyGUICommon)
if (MYGUI_RENDERSYSTEM EQUAL 10)
	find_program(MYGUI_PLATFORM_TEST_GLSLANG NAMES glslangValidator
			HINTS "$ENV{VULKAN_SDK}/bin" REQUIRED)
	set(shader_dir "${CMAKE_CURRENT_BINARY_DIR}/shaders")
	file(MAKE_DIRECTORY "${shader_dir}")
	add_custom_command(OUTPUT "${shader_dir}/PlatformSwap_Vulkan_FP.spv"
			COMMAND "${MYGUI_PLATFORM_TEST_GLSLANG}" -V
			"${MYGUI_SOURCE_DIR}/Media/UnitTests/UnitTest_Platform/PlatformSwap_Vulkan_FP.frag"
			-o "${shader_dir}/PlatformSwap_Vulkan_FP.spv"
			DEPENDS "${MYGUI_SOURCE_DIR}/Media/UnitTests/UnitTest_Platform/PlatformSwap_Vulkan_FP.frag"
			VERBATIM)
	add_custom_target(UnitTest_Platform_Shaders
			COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:UnitTest_Platform>/GeneratedShaders"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${shader_dir}/PlatformSwap_Vulkan_FP.spv"
			"$<TARGET_FILE_DIR:UnitTest_Platform>/GeneratedShaders/PlatformSwap_Vulkan_FP.spv"
			DEPENDS "${shader_dir}/PlatformSwap_Vulkan_FP.spv"
			VERBATIM)
	add_dependencies(UnitTest_Platform UnitTest_Platform_Shaders)
endif ()
foreach(group resources rendering lifecycle)
	add_test(NAME UnitTest_Platform_${group}
		COMMAND UnitTest_Platform --group ${group}
			--artifacts "${CMAKE_CURRENT_BINARY_DIR}/artifacts/${group}")
	set_tests_properties(UnitTest_Platform_${group} PROPERTIES
		LABELS "platform;graphics" RUN_SERIAL TRUE TIMEOUT 120
		WORKING_DIRECTORY "$<TARGET_FILE_DIR:UnitTest_Platform>")
endforeach()
