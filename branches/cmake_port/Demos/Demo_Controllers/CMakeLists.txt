
# define additional include directories
include_directories(.)

# define the sources
set( sources
	State.cpp
	State.h
	DemoKeeper.h
	DemoKeeper.cpp
)

# setup MyGUIEngine target
add_executable(demo_controllers ${sources})

# add dependencies
add_dependencies(demo_controllers MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(demo_controllers
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)

