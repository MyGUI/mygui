
# define additional include directories
include_directories(.)

# define the sources
set( sources
	CellView.cpp
	CellView.h
	ItemBox.cpp
	ItemBox.h
	ItemBoxWindow.h
	ItemBoxWindow.cpp
	ItemData.h
	ResourceItemInfo.cpp
	ResourceItemInfo.h
	ToolTip.cpp
	ToolTip.h
	DemoKeeper.cpp
	DemoKeeper.h
)

# setup MyGUIEngine target
add_executable(demo_itembox ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(demo_itembox PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(demo_itembox MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(demo_itembox
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)

MYGUI_APP(demo::DemoKeeper)
