#####################################################
# Install dependencies on Windows
#####################################################

function(install_dll_file DEBUG_FILEPATH RELEASE_FILEPATH FILENAME)
	if (EXISTS ${DEBUG_FILEPATH}${FILENAME}_d.dll)
		install(FILES
			${DEBUG_FILEPATH}${FILENAME}_d.dll
			DESTINATION bin/debug CONFIGURATIONS Debug
		)
		install(FILES
			${RELEASE_FILEPATH}${FILENAME}.dll
			DESTINATION bin/release CONFIGURATIONS Release None ""
		)
		install(FILES
			${RELEASE_FILEPATH}${FILENAME}.dll
			DESTINATION bin/relwithdebinfo CONFIGURATIONS RelWithDebInfo
		)
		install(FILES
			${RELEASE_FILEPATH}${FILENAME}.dll
			DESTINATION bin/minsizerel CONFIGURATIONS MinSizeRel
		)
		
		configure_file(${DEBUG_FILEPATH}${FILENAME}_d.dll ${MYGUI_BINARY_DIR}/bin/debug/${FILENAME}_d.dll COPYONLY)
		configure_file(${RELEASE_FILEPATH}${FILENAME}.dll ${MYGUI_BINARY_DIR}/bin/release/${FILENAME}.dll COPYONLY)
		configure_file(${RELEASE_FILEPATH}${FILENAME}.dll ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/${FILENAME}.dll COPYONLY)
		configure_file(${RELEASE_FILEPATH}${FILENAME}.dll ${MYGUI_BINARY_DIR}/bin/minsizerel/${FILENAME}.dll COPYONLY)
	endif ()
endfunction(install_dll_file)

if (MYGUI_DEPENDENCIES_DIR STREQUAL "")
	set(MYGUI_DEP_BIN_DIR ${MYGUI_SOURCE_DIR}/Dependencies/bin)
else ()
	set(MYGUI_DEP_BIN_DIR ${MYGUI_DEPENDENCIES_DIR}/bin)
endif ()

option(MYGUI_TRY_TO_COPY_DLLS "Copy dlls needed for sample builds" TRUE)

if (MYGUI_TRY_TO_COPY_DLLS)
	# copy the dependency DLLs to the right places
	if (MYGUI_SAMPLES_INPUT EQUAL 1)
		install_dll_file(${MYGUI_DEP_BIN_DIR}/debug/ ${MYGUI_DEP_BIN_DIR}/release/ OIS)
	elseif (MYGUI_SAMPLES_INPUT EQUAL 2)
	elseif (MYGUI_SAMPLES_INPUT EQUAL 3)
		install_dll_file(${MYGUI_DEP_BIN_DIR}/debug/ ${MYGUI_DEP_BIN_DIR}/release/ OIS)
	endif ()
	
	if (MYGUI_RENDERSYSTEM EQUAL 3)
		if (EXISTS ${OGRE_LIB_DIR}/../bin/debug/OgreMain_d.dll)
			set(DEBUG_DLLS_DIR ${OGRE_LIB_DIR}/../bin/debug)
			set(RELEASE_DLLS_DIR ${OGRE_LIB_DIR}/../bin/release)
			MESSAGE(STATUS "    DLL's were copied from bin")
		elseif (EXISTS "${OGRE_LIB_DIR}/OgreMain_d.dll")
			set(DEBUG_DLLS_DIR ${OGRE_LIB_DIR})
			set(RELEASE_DLLS_DIR ${OGRE_LIB_DIR})
			MESSAGE(STATUS "    DLL's were copied from lib")
		elseif (EXISTS ${OGRE_SOUCE_DIR}/bin/debug/OgreMain_d.dll)
			set(DEBUG_DLLS_DIR ${OGRE_SOUCE_DIR}/bin/debug)
			set(RELEASE_DLLS_DIR ${OGRE_SOUCE_DIR}/bin/release)
			MESSAGE(STATUS "    DLL's were copied from sdk's bin")
		elseif (EXISTS ${OGRE_BUILD}/bin/debug/OgreMain_d.dll)
			set(DEBUG_DLLS_DIR ${OGRE_BUILD}/bin/debug)
			set(RELEASE_DLLS_DIR ${OGRE_BUILD}/bin/release)
			MESSAGE(STATUS "    DLL's were copied from build bin")
		else ()
			set(DEBUG_DLLS_DIR "")
			set(RELEASE_DLLS_DIR "")
			MESSAGE(ERROR "    compiled OGRE DLL's wasn't found")
		endif ()
		
		install_dll_file(${DEBUG_DLLS_DIR}/ ${RELEASE_DLLS_DIR}/ OgreMain)
		install_dll_file(${DEBUG_DLLS_DIR}/ ${RELEASE_DLLS_DIR}/ RenderSystem_Direct3D9)
		install_dll_file(${DEBUG_DLLS_DIR}/ ${RELEASE_DLLS_DIR}/ RenderSystem_GL)
	endif ()
endif ()
