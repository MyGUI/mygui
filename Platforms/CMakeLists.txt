foreach(_rs IN LISTS MYGUI_BUILD_RENDERSYSTEMS)
	mygui_set_platform_name(${_rs})

	add_subdirectory(${MYGUI_PLATFORM_NAME}Platform)

	if(TARGET MyGUI.${MYGUI_PLATFORM_NAME}Platform)
		set_target_properties(MyGUI.${MYGUI_PLATFORM_NAME}Platform PROPERTIES
			EXPORT_NAME ${MYGUI_PLATFORM_NAME}Platform
		)
		add_library(MyGUI::${MYGUI_PLATFORM_NAME}Platform ALIAS MyGUI.${MYGUI_PLATFORM_NAME}Platform)
	endif()
endforeach()
