mygui_unit_test(UnitTest_VulkanPlatform)
target_link_libraries(UnitTest_VulkanPlatform PRIVATE MyGUICommon)
target_include_directories(UnitTest_VulkanPlatform PRIVATE ../UnitTest_Platform)
set_tests_properties(UnitTest_VulkanPlatform PROPERTIES
	LABELS "platform;graphics" RUN_SERIAL TRUE TIMEOUT 120
	WORKING_DIRECTORY "$<TARGET_FILE_DIR:UnitTest_VulkanPlatform>")
