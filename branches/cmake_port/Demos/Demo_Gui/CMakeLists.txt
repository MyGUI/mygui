
# define additional include directories
include_directories(.)

# define the sources
set( sources
	ColourWindow.cpp
	ColourWindow.h
	ColourWindowBox.cpp
	ColourWindowBox.h
	ColourWindowCellData.h
	ColourWindowCellView.cpp
	ColourWindowCellView.h
	EditorWindow.cpp
	EditorWindow.h
	InformationWindow.cpp
	InformationWindow.h
	MainPanel.cpp
	MainPanel.h
	DemoKeeper.cpp
	DemoKeeper.h
)

# setup MyGUIEngine target
add_executable(demo_gui ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(demo_gui PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(demo_gui MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(demo_gui
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)

