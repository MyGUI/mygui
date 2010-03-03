mygui_tool(LayoutEditor)

add_dependencies(LayoutEditor Plugin_StrangeButton)

if (MYGUI_BUILD_HIKARI_PLUGIN)
	if(WIN32)
		add_dependencies(LayoutEditor Plugin_HikariWidget)
	endif()
endif()

if (MYGUI_BUILD_AWESOMIUM_PLUGIN)
	if (WIN32 OR APPLE)
		add_dependencies(LayoutEditor Plugin_AwesomiumWidget)
	endif ()
endif()

if (MYGUI_BUILD_BERKELIUM_PLUGIN)
	add_dependencies(LayoutEditor Plugin_BerkeliumWidget)
endif()
