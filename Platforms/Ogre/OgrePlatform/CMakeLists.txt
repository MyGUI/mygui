set(PROJECTNAME MyGUI.OgrePlatform)

include_directories(
	include
	${MYGUI_SOURCE_DIR}/MyGUIEngine/include
)

include(${PROJECTNAME}.list)

add_library(${PROJECTNAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECTNAME} SYSTEM PUBLIC ${OGRE_INCLUDE_DIR})

add_dependencies(${PROJECTNAME} MyGUIEngine)

target_link_directories(${PROJECTNAME} PUBLIC ${OGRE_LIB_DIR} ${CMAKE_INSTALL_PREFIX}/lib)
target_link_libraries(${PROJECTNAME} PUBLIC ${OGRE_LIBRARIES} MyGUIEngine)

# installation rules
install(FILES ${HEADER_FILES}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
)
mygui_install_target(${PROJECTNAME} "")
