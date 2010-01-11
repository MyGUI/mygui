#####################################################
# Install dependencies on Windows
#####################################################

if (NOT WIN32)
	return()
endif()

set(MYGUI_DEP_CUR_DIR ${MYGUI_DEPENDENCIES_DIR})
set(MYGUI_DEP_BIN_DIR ${MYGUI_DEPENDENCIES_DIR}/bin)

option(MYGUI_TRY_TO_COPY_DLLS "Copy dlls needed for sample builds" FALSE)

if (MYGUI_TRY_TO_COPY_DLLS)
	if (MYGUI_STATIC)
		# for static builds, projects must link against all MyGUI dependencies themselves, so copy full include and lib dir
		install(DIRECTORY ${MYGUI_DEP_CUR_DIR}/include/ DESTINATION include)
		install(DIRECTORY ${MYGUI_DEP_CUR_DIR}/lib/ DESTINATION lib)
	else ()
		# for non-static builds, we only need OIS for the samples
		install(DIRECTORY ${MYGUI_DEP_CUR_DIR}/include/OIS DESTINATION include)
		install(FILES
		${MYGUI_DEP_CUR_DIR}/lib/debug/OIS_d.lib
		DESTINATION lib/debug CONFIGURATIONS Debug
		)
		install(FILES
		${MYGUI_DEP_CUR_DIR}/lib/release/OIS.lib
		DESTINATION lib/release CONFIGURATIONS Release RelWithDebInfo MinSizeRel None ""
		)
	endif ()
	
	if (EXISTS ${OGRE_LIB_DIR}/../bin/debug/OgreMain_d.dll)
		set(DEBUG_DLLS_DIR ${OGRE_LIB_DIR}/../bin/debug)
		set(RELEASE_DLLS_DIR ${OGRE_LIB_DIR}/../bin/release)
		elseif (EXISTS "${OGRE_LIB_DIR}/OgreMain_d.dll")
		set(DEBUG_DLLS_DIR ${OGRE_LIB_DIR})
		set(RELEASE_DLLS_DIR ${OGRE_LIB_DIR})
	else ()
		return()
	endif ()
	
	# copy the dependency DLLs to the right places
	install(FILES
		${MYGUI_DEP_BIN_DIR}/debug/OIS_d.dll
		${DEBUG_DLLS_DIR}/OgreMain_d.dll
		${DEBUG_DLLS_DIR}/RenderSystem_Direct3D9_d.dll
		${DEBUG_DLLS_DIR}/RenderSystem_GL_d.dll
		DESTINATION bin/debug CONFIGURATIONS Debug
	)
	install(FILES
		${MYGUI_DEP_BIN_DIR}/release/OIS.dll
		${RELEASE_DLLS_DIR}/OgreMain.dll
		${RELEASE_DLLS_DIR}/RenderSystem_Direct3D9.dll
		${RELEASE_DLLS_DIR}/RenderSystem_GL.dll
		DESTINATION bin/release CONFIGURATIONS Release None ""
	)
	install(FILES
		${MYGUI_DEP_BIN_DIR}/release/OIS.dll
		${RELEASE_DLLS_DIR}/OgreMain.dll
		${RELEASE_DLLS_DIR}/RenderSystem_Direct3D9.dll
		${RELEASE_DLLS_DIR}/RenderSystem_GL.dll
		DESTINATION bin/relwithdebinfo CONFIGURATIONS RelWithDebInfo
	)
	install(FILES
		${MYGUI_DEP_BIN_DIR}/release/OIS.dll
		${RELEASE_DLLS_DIR}/OgreMain.dll
		${RELEASE_DLLS_DIR}/RenderSystem_Direct3D9.dll
		${RELEASE_DLLS_DIR}/RenderSystem_GL.dll
		DESTINATION bin/minsizerel CONFIGURATIONS MinSizeRel
	)


	configure_file(${MYGUI_DEP_BIN_DIR}/debug/OIS_d.dll ${MYGUI_BINARY_DIR}/bin/debug/OIS_d.dll COPYONLY)
	configure_file(${DEBUG_DLLS_DIR}/OgreMain_d.dll ${MYGUI_BINARY_DIR}/bin/debug/OgreMain_d.dll COPYONLY)
	configure_file(${DEBUG_DLLS_DIR}/RenderSystem_Direct3D9_d.dll ${MYGUI_BINARY_DIR}/bin/debug/RenderSystem_Direct3D9_d.dll COPYONLY)
	configure_file(${DEBUG_DLLS_DIR}/RenderSystem_GL_d.dll ${MYGUI_BINARY_DIR}/bin/debug/RenderSystem_GL_d.dll COPYONLY)

	configure_file(${MYGUI_DEP_BIN_DIR}/release/OIS.dll ${MYGUI_BINARY_DIR}/bin/release/OIS.dll COPYONLY)
	configure_file(${RELEASE_DLLS_DIR}/OgreMain.dll ${MYGUI_BINARY_DIR}/bin/release/OgreMain.dll COPYONLY)
	configure_file(${RELEASE_DLLS_DIR}/RenderSystem_Direct3D9.dll ${MYGUI_BINARY_DIR}/bin/release/RenderSystem_Direct3D9.dll COPYONLY)
	configure_file(${RELEASE_DLLS_DIR}/RenderSystem_GL.dll ${MYGUI_BINARY_DIR}/bin/release/RenderSystem_GL.dll COPYONLY)

	configure_file(${MYGUI_DEP_BIN_DIR}/release/OIS.dll ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/OIS.dll COPYONLY)
	configure_file(${RELEASE_DLLS_DIR}/OgreMain.dll ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/OgreMain.dll COPYONLY)
	configure_file(${RELEASE_DLLS_DIR}/RenderSystem_Direct3D9.dll ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/RenderSystem_Direct3D9.dll COPYONLY)
	configure_file(${RELEASE_DLLS_DIR}/RenderSystem_GL.dll ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/RenderSystem_GL.dll COPYONLY)

	configure_file(${MYGUI_DEP_BIN_DIR}/release/OIS.dll ${MYGUI_BINARY_DIR}/bin/minsizerel/OIS.dll COPYONLY)
	configure_file(${RELEASE_DLLS_DIR}/OgreMain.dll ${MYGUI_BINARY_DIR}/bin/minsizerel/OgreMain.dll COPYONLY)
	configure_file(${RELEASE_DLLS_DIR}/RenderSystem_Direct3D9.dll ${MYGUI_BINARY_DIR}/bin/minsizerel/RenderSystem_Direct3D9.dll COPYONLY)
	configure_file(${RELEASE_DLLS_DIR}/RenderSystem_GL.dll ${MYGUI_BINARY_DIR}/bin/minsizerel/RenderSystem_GL.dll COPYONLY)
endif ()
