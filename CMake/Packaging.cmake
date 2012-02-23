option(MYGUI_CREATE_PACKAGE "Create package." FALSE)

if (MYGUI_CREATE_PACKAGE)

	set(CPACK_PACKAGE_VERSION ${MYGUI_VERSION})
	set(CPACK_PACKAGE_VERSION_MAJOR ${MYGUI_VERSION_MAJOR})
	set(CPACK_PACKAGE_VERSION_MINOR ${MYGUI_VERSION_MINOR})
	set(CPACK_PACKAGE_VERSION_PATCH ${MYGUI_VERSION_PATCH})

	set(CPACK_INSTALL_CMAKE_PROJECTS "${MYGUI_BINARY_DIR}" "MYGUI" "ALL" "/")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MyGUI - fast, flexible and simple GUI")
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "MYGUI")
	set(CPACK_PACKAGE_NAME "MYGUI")
	set(CPACK_PACKAGE_VENDOR "")

	set(CPACK_RESOURCE_FILE_LICENSE "${MYGUI_SOURCE_DIR}/COPYING.LESSER")

	set(CPACK_PACKAGE_CONTACT "MYGUI Team <altren@gmail.com>")

	if (WIN32 AND NOT UNIX)
		set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr \\\${WriteEnvStr_RegKey} \\\"MYGUI_HOME\\\" $INSTDIR")
		
		# There is a bug in NSIS that does not handle full unix paths properly. Make
		# sure there is at least one set of four (4) backlasshes.
		SET(CPACK_PACKAGE_ICON "${MYGUI_SOURCE_DIR}/Media/Common/Sources\\\\MyGUI_Logo.bmp")
		SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin/Debug\\\\LayoutEditor.exe")
		SET(CPACK_NSIS_MUI_ICON  "Common/Base\\\\mygui.ico")
		SET(CPACK_NSIS_DISPLAY_NAME "MyGUI")
		SET(CPACK_NSIS_HELP_LINK "http://my-gui.sourceforge.net/")

	#	SET(CPACK_NSIS_MENU_LINKS
	#		"doc/cmake-@CMake_VERSION_MAJOR@.@CMake_VERSION_MINOR@/cmake-gui.html" "cmake-gui Help"
	#		"doc/cmake-@CMake_VERSION_MAJOR@.@CMake_VERSION_MINOR@/cmake.html" "CMake Help"
	#		"doc/cmake-@CMake_VERSION_MAJOR@.@CMake_VERSION_MINOR@/cmake-properties.html"
	#		"CMake Properties and Variables Help"
	#		"doc/cmake-@CMake_VERSION_MAJOR@.@CMake_VERSION_MINOR@/ctest.html" "CTest Help"
	#		"doc/cmake-@CMake_VERSION_MAJOR@.@CMake_VERSION_MINOR@/cmake-modules.html" "CMake Modules Help"
	#		"doc/cmake-@CMake_VERSION_MAJOR@.@CMake_VERSION_MINOR@/cmake-commands.html" "CMake Commands Help"
	#		"doc/cmake-@CMake_VERSION_MAJOR@.@CMake_VERSION_MINOR@/cpack.html" "CPack Help"
	#		"http://MyGUI.info" "MyGUI Web Site"
	#	)
	endif ()

	include(CPack)

endif ()
