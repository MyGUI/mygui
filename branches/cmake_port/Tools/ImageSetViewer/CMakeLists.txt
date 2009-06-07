
# define additional include directories
include_directories(.)

# define the sources
set( sources
	Controller.cpp
	Controller.h
	View.h
	View.cpp
)

# setup MyGUIEngine target
add_executable(imageset_editor ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(imageset_editor PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(imageset_editor MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(imageset_editor
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)

