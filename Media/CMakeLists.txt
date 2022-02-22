###############################################################
# Install media
###############################################################

# Set media target directory
if (WIN32)
  set(MYGUI_MEDIA_PATH "Media")
elseif (UNIX)
  set(MYGUI_MEDIA_PATH "share/MYGUI/Media")
endif ()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${MYGUI_MEDIA_PATH}
  PATTERN "CMakeLists.txt" EXCLUDE
  PATTERN "vector" EXCLUDE
)
