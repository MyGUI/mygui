set(PROJECTNAME MyGUI.OpenGL3Platform)

include(${PROJECTNAME}.list)

add_library(${PROJECTNAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECTNAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/MYGUI>
	PRIVATE
		"${MYGUI_SOURCE_DIR}/Platforms/Utils"
)

target_compile_definitions(${PROJECTNAME} PRIVATE GL_GLEXT_PROTOTYPES)

if (APPLE)
	target_compile_definitions(${PROJECTNAME} PUBLIC GL_SILENCE_DEPRECATION)
endif()

target_link_libraries(${PROJECTNAME}
	PUBLIC
		MyGUI::MyGUI
	PRIVATE
		OpenGL::GL
)

# installation rules
install(FILES ${HEADER_FILES}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
)
mygui_install_target(${PROJECTNAME})
