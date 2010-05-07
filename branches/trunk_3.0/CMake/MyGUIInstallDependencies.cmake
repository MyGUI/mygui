#####################################################
# Install dependencies on Windows
#####################################################

function(install_dll_file DEBUG_FILEPATH RELEASE_SEFILEPATH FILENAME)
	install(FILES
		${DEBUG_FILEPATH}${FILENAME}_d.dll
		DESTINATION bin/debug CONFIGURATIONS Debug
	)
	install(FILES
		${RELEASE_SEFILEPATH}${FILENAME}.dll
		DESTINATION bin/release CONFIGURATIONS Release None ""
	)
	install(FILES
		${RELEASE_SEFILEPATH}${FILENAME}.dll
		DESTINATION bin/relwithdebinfo CONFIGURATIONS RelWithDebInfo
	)
	install(FILES
		${RELEASE_SEFILEPATH}${FILENAME}.dll
		DESTINATION bin/minsizerel CONFIGURATIONS MinSizeRel
	)
	
	configure_file(${DEBUG_FILEPATH}${FILENAME}_d.dll ${MYGUI_BINARY_DIR}/bin/debug/${FILENAME}_d.dll COPYONLY)
	configure_file(${RELEASE_SEFILEPATH}${FILENAME}.dll ${MYGUI_BINARY_DIR}/bin/release/${FILENAME}.dll COPYONLY)
	configure_file(${RELEASE_SEFILEPATH}${FILENAME}.dll ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/${FILENAME}.dll COPYONLY)
	configure_file(${RELEASE_SEFILEPATH}${FILENAME}.dll ${MYGUI_BINARY_DIR}/bin/minsizerel/${FILENAME}.dll COPYONLY)
endfunction(install_dll_file)

set(MYGUI_DEP_CUR_DIR ${MYGUI_DEPENDENCIES_DIR})
set(MYGUI_DEP_BIN_DIR ${MYGUI_DEPENDENCIES_DIR}/bin)

option(MYGUI_TRY_TO_COPY_DLLS "Copy dlls needed for sample builds" FALSE)

if (MYGUI_TRY_TO_COPY_DLLS)
	if (MYGUI_RENDERSYSTEM EQUAL 2)
		if (EXISTS ${OGRE_LIB_DIR}/../bin/debug/OgreMain_d.dll)
			set(DEBUG_DLLS_DIR ${OGRE_LIB_DIR}/../bin/debug)
			set(RELEASE_DLLS_DIR ${OGRE_LIB_DIR}/../bin/release)
		elseif (EXISTS "${OGRE_LIB_DIR}/OgreMain_d.dll")
			set(DEBUG_DLLS_DIR ${OGRE_LIB_DIR})
			set(RELEASE_DLLS_DIR ${OGRE_LIB_DIR})
		else ()
			return()
		endif ()
	endif()
	
	# copy the dependency DLLs to the right places
	install_dll_file(${MYGUI_DEP_BIN_DIR}/debug/ ${MYGUI_DEP_BIN_DIR}/release/ OIS)
	
	if (MYGUI_RENDERSYSTEM EQUAL 2)
		install_dll_file(${DEBUG_DLLS_DIR}/ ${RELEASE_DLLS_DIR}/ OgreMain)
		install_dll_file(${DEBUG_DLLS_DIR}/ ${RELEASE_DLLS_DIR}/ RenderSystem_Direct3D9)
		install_dll_file(${DEBUG_DLLS_DIR}/ ${RELEASE_DLLS_DIR}/ RenderSystem_GL)
	endif ()
endif ()
