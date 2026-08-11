set(PROJECTNAME MyGUI.OsgPlatform)

include(${PROJECTNAME}.list)

add_library(${PROJECTNAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECTNAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/MYGUI>
	PRIVATE
		"${MYGUI_SOURCE_DIR}/Platforms/Utils"
)

if(MYGUI_OSG_PLATFORM_TARGETS)
	target_link_libraries(${PROJECTNAME}
		PUBLIC
			${MYGUI_OSG_PLATFORM_TARGETS}
	)
	# the osg targets from vcpkg do not provide the include directory themselves
	if(DEFINED VCPKG_INSTALLED_DIR)
		target_include_directories(${PROJECTNAME} SYSTEM PUBLIC
			$<BUILD_INTERFACE:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include>
		)
	endif()
endif()
if(MYGUI_OSG_INCLUDE_DIR)
	target_include_directories(${PROJECTNAME} SYSTEM PUBLIC "${MYGUI_OSG_INCLUDE_DIR}")
	target_link_libraries(${PROJECTNAME}
		PUBLIC
			${MYGUI_OSG_PLATFORM_LIBRARIES}
	)
endif()

target_link_libraries(${PROJECTNAME}
	PUBLIC
		MyGUI::MyGUI
)

# installation rules
install(FILES ${HEADER_FILES}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MYGUI"
)
mygui_install_target(${PROJECTNAME})
