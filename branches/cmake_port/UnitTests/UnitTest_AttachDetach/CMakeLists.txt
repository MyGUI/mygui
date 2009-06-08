
# define additional include directories
include_directories(.)

# define the sources
set( sources
	DemoKeeper.h
	DemoKeeper.cpp
)

# setup MyGUIEngine target
add_executable(test_attach_detach ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(test_attach_detach PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(test_attach_detach MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(test_attach_detach
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)

