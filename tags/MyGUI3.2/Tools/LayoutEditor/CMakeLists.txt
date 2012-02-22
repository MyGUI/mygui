mygui_tool(LayoutEditor)

if (MYGUI_BUILD_PLUGINS)
	add_dependencies(LayoutEditor Plugin_StrangeButton)

	if (MYGUI_BUILD_HIKARI_PLUGIN)
		if(WIN32)
			add_dependencies(LayoutEditor Plugin_HikariWidget)
		endif()
	endif()

	if (MYGUI_BUILD_BERKELIUM_PLUGIN)
		add_dependencies(LayoutEditor Plugin_BerkeliumWidget)
	endif()
endif()
