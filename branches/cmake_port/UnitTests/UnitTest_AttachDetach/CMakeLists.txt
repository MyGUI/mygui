set (PROJECT_NAME UnitTest_AttachDetach)
# define additional include directories
include_directories(.)

# define the sources
include(${PROJECT_NAME}.list)

# setup MyGUIEngine target
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# set some platform specific things
if(WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(${PROJECT_NAME} MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

mygui_config_common(${PROJECT_NAME})

# link libraries against it
target_link_libraries(${PROJECT_NAME}
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)
