
# define additional include directories
include_directories(.)

# define the sources
set( sources
	FontPanel.cpp
	FontPanel.h
	FontView.cpp
	FontView.h
	TextureView.cpp
	TextureView.h
	DemoKeeper.h
	DemoKeeper.cpp
)

# setup MyGUIEngine target
add_executable(demo_font ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(demo_font PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(demo_font MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(demo_font
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)

