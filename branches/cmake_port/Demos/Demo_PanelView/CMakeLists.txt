
# define additional include directories
include_directories(.)

# define the sources
set( sources
	PanelDirector.cpp
	PanelDirector.h
	PanelDynamic.cpp
	PanelDynamic.h
	PanelStatic.h
	PanelStatic.cpp
	PanelView.h
	PanelViewCell.h
	PanelViewWindow.h
	DemoKeeper.cpp
	DemoKeeper.h
)

# setup MyGUIEngine target
add_executable(demo_panelview ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(demo_panelview PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(demo_panelview MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(demo_panelview
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)
