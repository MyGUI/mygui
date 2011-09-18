#############################################################
# Install useful CMake modules.
# These are necessary to compile the samples from the install
# directory, but can also be used for custom projects.
#############################################################

if(WIN32)
	set(MYGUI_CMAKE_DIR "CMake")
else(WIN32)
	set(MYGUI_CMAKE_DIR "lib/MYGUI/cmake")
endif(WIN32)

set(INST_FILES
  Packages/FindOIS.cmake
  Packages/FindOGRE.cmake
  Utils/FindPkgMacros.cmake
  Utils/PreprocessorUtils.cmake
  Utils/MyGUIConfigTargets.cmake
  Utils/MyGUIGetVersion.cmake
  Utils/MyGUIFindFrameworks.cmake
  Templates/VisualStudioUserFile.vcproj.user.in
  Templates/VisualStudio2010UserFile.vcxproj.user.in
) 
set(STATIC_INST_FILES
  Packages/FindDirectX.cmake
  Packages/FindDirectX11.cmake
  Packages/FindFreetype.cmake
)
if (APPLE)
  set(INST_FILES ${INST_FILES}
    Packages/FindIOKit.cmake
  )
  set(STATIC_INST_FILES ${STATIC_INST_FILES}
    Packages/FindCarbon.cmake
    Packages/FindCocoa.cmake
  )
endif ()

#install(FILES ${INST_FILES} DESTINATION ${MYGUI_CMAKE_DIR})
if (MYGUI_STATIC)
  install(FILES ${STATIC_INST_FILES} DESTINATION ${MYGUI_CMAKE_DIR})
endif ()