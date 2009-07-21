##################################################################
# Generate and install the config files needed for the samples
##################################################################

# install resource files
if (MYGUI_INSTALL_SAMPLES)
  install(FILES 
    ${MYGUI_BINARY_DIR}/bin/debug/resources.cfg
    ${MYGUI_BINARY_DIR}/bin/debug/plugins.cfg
    DESTINATION "bin${MYGUI_DEBUG_PATH}" CONFIGURATIONS Debug
  )
  install(FILES 
    ${MYGUI_BINARY_DIR}/bin/release/resources.cfg
    ${MYGUI_BINARY_DIR}/bin/release/plugins.cfg
    DESTINATION "bin${MYGUI_RELEASE_PATH}" CONFIGURATIONS Release None ""
  )
  install(FILES 
    ${MYGUI_BINARY_DIR}/bin/release/resources.cfg
    ${MYGUI_BINARY_DIR}/bin/release/plugins.cfg
    DESTINATION "bin${MYGUI_RELWDBG_PATH}" CONFIGURATIONS RelWithDebInfo
  )
  install(FILES 
    ${MYGUI_BINARY_DIR}/bin/release/resources.cfg
    ${MYGUI_BINARY_DIR}/bin/release/plugins.cfg
    DESTINATION "bin${MYGUI_MINSIZE_PATH}" CONFIGURATIONS MinSizeRel
  )
endif ()


