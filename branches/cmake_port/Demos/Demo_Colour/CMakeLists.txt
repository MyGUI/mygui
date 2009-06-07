
# define additional include directories
include_directories(.)

# define the sources
set( sources
	EditPanel.cpp
	ColourPanel.h
	DemoKeeper.h
	ColourPanel.cpp
	DemoKeeper.cpp
	EditPanel.h
)

# setup MyGUIEngine target
add_executable(demo_colour ${sources})

# add dependencies
add_dependencies(demo_colour MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(demo_colour
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)

