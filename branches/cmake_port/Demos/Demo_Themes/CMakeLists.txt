
# define additional include directories
include_directories(.)

# define the sources
set( sources
	DemoKeeper.cpp
	DemoKeeper.h
)

# setup MyGUIEngine target
add_executable(demo_themes ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(demo_themes PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(demo_themes MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(demo_themes
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)
