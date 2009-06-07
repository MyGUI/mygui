
# define additional include directories
include_directories(.)

# define the sources
set( sources
	Console.cpp
	Console.h
	DemoKeeper.h
	DemoKeeper.cpp
)

# setup MyGUIEngine target
add_executable(demo_console ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(demo_console PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(demo_console MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(demo_console
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)

