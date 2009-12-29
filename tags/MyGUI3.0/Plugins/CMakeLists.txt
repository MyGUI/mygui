add_subdirectory(Plugin_StrangeButton)

if(WIN32)
	add_subdirectory(Plugin_HikariWidget)
endif(WIN32)

if(WIN32 OR APPLE)
	add_subdirectory(Plugin_AwesomiumWidget)
endif(WIN32 OR APPLE)
