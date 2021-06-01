set(PROJECTNAME MyGUI.OpenGL3Platform)

include_directories(
	include
	${MYGUI_SOURCE_DIR}/MyGUIEngine/include
	${OPENGL_INCLUDE_DIR}
	${MYGUI_SOURCE_DIR}/Common
)

include_directories(SYSTEM include/GL)

include(${PROJECTNAME}.list)
if (NOT MYGUI_USE_SYSTEM_GLEW)
	set(SOURCE_FILES ${SOURCE_FILES} include/GL/glew.c)
endif ()

if (NOT MYGUI_USE_SYSTEM_GLEW)
	add_definitions(-DGLEW_STATIC)
endif ()
add_definitions(-DGL_GLEXT_PROTOTYPES)

add_library(${PROJECTNAME} ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies(${PROJECTNAME} MyGUIEngine)

target_link_libraries(${PROJECTNAME} ${OPENGL_opengl_LIBRARY} MyGUIEngine)
if (MYGUI_USE_SYSTEM_GLEW)
	target_link_libraries(${PROJECTNAME} ${GLEW_LIBRARIES})
else ()
	target_link_libraries(${PROJECTNAME} ${OPENGL_glx_LIBRARY})
endif ()

link_directories(${OPENGL_LIB_DIR})

# installation rules
install(FILES ${HEADER_FILES}
	DESTINATION include/MYGUI
)
mygui_install_target(${PROJECTNAME} "")

if (MYGUI_HIGH_LEVEL_WARNINGS)
	if (NOT MSVC)
		set_source_files_properties(include/GL/glew.c PROPERTIES COMPILE_FLAGS -w)
		set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS -Wno-reserved-id-macro)
	endif ()
endif ()
