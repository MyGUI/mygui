option(MYGUI_BUILD_AWESOMIUM_PLUGIN "Build Plugin AwesomiumWidget" FALSE)
option(MYGUI_BUILD_HIKARI_PLUGIN "Build Plugin HikariWidget" FALSE)

add_subdirectory(Plugin_StrangeButton)

if (MYGUI_BUILD_HIKARI_PLUGIN)
	if(WIN32)
		add_subdirectory(Plugin_HikariWidget)
	endif()
endif()

if (MYGUI_BUILD_AWESOMIUM_PLUGIN)
	if (WIN32 OR APPLE)
		add_subdirectory(Plugin_AwesomiumWidget)
	endif ()
endif()
