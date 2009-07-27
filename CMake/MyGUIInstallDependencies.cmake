#####################################################
# Install dependencies on Windows
#####################################################

if (NOT WIN32)
  return()
endif()

set(MYGUI_DEP_CUR_DIR ${MYGUI_DEPENDENCIES_DIR})
set(MYGUI_DEP_BIN_DIR ${MYGUI_DEPENDENCIES_DIR}/../Samples/Common/bin)

option(MYGUI_INSTALL_DEPENDENCIES "Install dependencies needed for sample builds" FALSE)

if (MYGUI_INSTALL_DEPENDENCIES)
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
    
  # copy the dependency DLLs to the right places
  install(FILES
    ${MYGUI_DEP_BIN_DIR}/debug/OIS_d.dll
    ${OGRE_LIB_DIR}/../bin/debug/OgreMain_d.dll
    ${OGRE_LIB_DIR}/../bin/debug/RenderSystem_Direct3D9_d.dll
    ${OGRE_LIB_DIR}/../bin/debug/RenderSystem_GL_d.dll
	  DESTINATION bin/debug CONFIGURATIONS Debug
  )
  install(FILES
    ${MYGUI_DEP_BIN_DIR}/release/OIS.dll
    ${OGRE_LIB_DIR}/../bin/release/OgreMain.dll
    ${OGRE_LIB_DIR}/../bin/release/RenderSystem_Direct3D9.dll
    ${OGRE_LIB_DIR}/../bin/release/RenderSystem_GL.dll
	  DESTINATION bin/release CONFIGURATIONS Release None ""
  )  
  install(FILES
    ${MYGUI_DEP_BIN_DIR}/release/OIS.dll
    ${OGRE_LIB_DIR}/../bin/release/OgreMain.dll
    ${OGRE_LIB_DIR}/../bin/release/RenderSystem_Direct3D9.dll
    ${OGRE_LIB_DIR}/../bin/release/RenderSystem_GL.dll
	  DESTINATION bin/relwithdebinfo CONFIGURATIONS RelWithDebInfo
  )  
  install(FILES
    ${MYGUI_DEP_BIN_DIR}/release/OIS.dll
    ${OGRE_LIB_DIR}/../bin/release/OgreMain.dll
    ${OGRE_LIB_DIR}/../bin/release/RenderSystem_Direct3D9.dll
    ${OGRE_LIB_DIR}/../bin/release/RenderSystem_GL.dll
	  DESTINATION bin/minsizerel CONFIGURATIONS MinSizeRel
  )
endif ()

if (MYGUI_INSTALL_DEPENDENCIES)
	configure_file(${MYGUI_DEP_BIN_DIR}/debug/OIS_d.dll ${MYGUI_BINARY_DIR}/bin/debug/OIS_d.dll COPYONLY)
	configure_file(${OGRE_LIB_DIR}/../bin/debug/OgreMain_d.dll ${MYGUI_BINARY_DIR}/bin/debug/OgreMain_d.dll COPYONLY)
	configure_file(${OGRE_LIB_DIR}/../bin/debug/RenderSystem_Direct3D9_d.dll ${MYGUI_BINARY_DIR}/bin/debug/RenderSystem_Direct3D9_d.dll COPYONLY)
	configure_file(${OGRE_LIB_DIR}/../bin/debug/RenderSystem_GL_d.dll ${MYGUI_BINARY_DIR}/bin/debug/RenderSystem_GL_d.dll COPYONLY)

	configure_file(${MYGUI_DEP_BIN_DIR}/release/OIS.dll ${MYGUI_BINARY_DIR}/bin/release/OIS.dll COPYONLY)
	configure_file(${OGRE_LIB_DIR}/../bin/release/OgreMain.dll ${MYGUI_BINARY_DIR}/bin/release/OgreMain.dll COPYONLY)
	configure_file(${OGRE_LIB_DIR}/../bin/release/RenderSystem_Direct3D9.dll ${MYGUI_BINARY_DIR}/bin/release/RenderSystem_Direct3D9.dll COPYONLY)
	configure_file(${OGRE_LIB_DIR}/../bin/release/RenderSystem_GL.dll ${MYGUI_BINARY_DIR}/bin/release/RenderSystem_GL.dll COPYONLY)

	configure_file(${MYGUI_DEP_BIN_DIR}/release/OIS.dll ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/OIS.dll COPYONLY)
	configure_file(${OGRE_LIB_DIR}/../bin/release/OgreMain.dll ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/OgreMain.dll COPYONLY)
	configure_file(${OGRE_LIB_DIR}/../bin/release/RenderSystem_Direct3D9.dll ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/RenderSystem_Direct3D9.dll COPYONLY)
	configure_file(${OGRE_LIB_DIR}/../bin/release/RenderSystem_GL.dll ${MYGUI_BINARY_DIR}/bin/relwithdebinfo/RenderSystem_GL.dll COPYONLY)

	configure_file(${MYGUI_DEP_BIN_DIR}/release/OIS.dll ${MYGUI_BINARY_DIR}/bin/minsizerel/OIS.dll COPYONLY)
	configure_file(${OGRE_LIB_DIR}/../bin/release/OgreMain.dll ${MYGUI_BINARY_DIR}/bin/minsizerel/OgreMain.dll COPYONLY)
	configure_file(${OGRE_LIB_DIR}/../bin/release/RenderSystem_Direct3D9.dll ${MYGUI_BINARY_DIR}/bin/minsizerel/RenderSystem_Direct3D9.dll COPYONLY)
	configure_file(${OGRE_LIB_DIR}/../bin/release/RenderSystem_GL.dll ${MYGUI_BINARY_DIR}/bin/minsizerel/RenderSystem_GL.dll COPYONLY)
endif ()
