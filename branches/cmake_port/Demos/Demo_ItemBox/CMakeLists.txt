
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
add_executable(demo_gui ${sources})

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

