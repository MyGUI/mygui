set(PROJECTNAME MyGUI.OgreNextPlatform)

include(${PROJECTNAME}.list)

add_library(${PROJECTNAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECTNAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/MYGUI>
)

target_link_libraries(${PROJECTNAME} PUBLIC OgreNext::OgreNext MyGUI::MyGUI)
if(OGRE_STATIC AND TARGET ZLIB::ZLIB)
	target_link_libraries(${PROJECTNAME} PUBLIC ZLIB::ZLIB)
endif()

# installation rules
install(FILES ${HEADER_FILES}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
)
mygui_install_target(${PROJECTNAME})

# platform specific dependencies
if(APPLE)
	target_include_directories(${PROJECTNAME} PRIVATE /usr/local/include /usr/include)
	target_link_libraries(${PROJECTNAME} ${Carbon_LIBRARY})
endif()
