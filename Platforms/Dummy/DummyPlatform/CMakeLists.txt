set(PROJECTNAME MyGUI.DummyPlatform)

include(${PROJECTNAME}.list)

add_library(${PROJECTNAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECTNAME}
	PUBLIC
		include
	PRIVATE
		"${MYGUI_SOURCE_DIR}/Common"
)

target_link_libraries(${PROJECTNAME} MyGUIEngine)

# installation rules
install(FILES ${HEADER_FILES}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
)
mygui_install_target(${PROJECTNAME})
