
# define additional include directories
include_directories(.)

# define the sources
set( sources
	StaticImageView.cpp
	StaticImageView.h
	DemoKeeper.cpp
	DemoKeeper.h
)

# setup MyGUIEngine target
add_executable(demo_staticimage ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(demo_staticimage PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(demo_staticimage MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(demo_staticimage
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)
