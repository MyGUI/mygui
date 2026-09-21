set(PROJECTNAME MyGUI.OpenGLESPlatform)

include(${PROJECTNAME}.list)
add_library(${PROJECTNAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECTNAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/MYGUI>
	PRIVATE
		"${MYGUI_SOURCE_DIR}/Platforms/Utils"
)
target_compile_definitions(${PROJECTNAME} PUBLIC GL_GLEXT_PROTOTYPES)

target_link_libraries(${PROJECTNAME}
	PUBLIC
		MyGUI::MyGUI
)

if(EMSCRIPTEN)
	target_link_libraries(${PROJECTNAME} PRIVATE OpenGL::GL)
else()
	target_link_libraries(${PROJECTNAME} PUBLIC MyGUI_GLES)
endif()

# installation rules
install(FILES ${HEADER_FILES}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
)
mygui_install_target(${PROJECTNAME})
