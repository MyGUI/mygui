set(PROJECTNAME MyGUI.DirectXPlatform)

include(${PROJECTNAME}.list)

add_library(${PROJECTNAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECTNAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/MYGUI>
	PRIVATE
		"${DirectX_INCLUDE_DIR}"
		"${MYGUI_SOURCE_DIR}/Common"
)

target_link_libraries(${PROJECTNAME}
	PUBLIC
		MyGUIEngine
	PRIVATE
		${DirectX_LIBRARIES}
)
target_link_directories(${PROJECTNAME} PRIVATE ${DirectX_LIBRARY_DIRS})

# installation rules
install(FILES ${HEADER_FILES}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
)
mygui_install_target(${PROJECTNAME})
