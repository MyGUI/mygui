
# define additional include directories
include_directories(.)

# define the sources
set( sources
	DemoKeeper.cpp
	DemoKeeper.h
)

# setup MyGUIEngine target
add_executable(demo_picking ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(demo_picking PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(demo_picking MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(demo_picking
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)
