
# define additional include directories
include_directories(.)

# define the sources
set( sources
	PanelMainProperties.h
	UndoManager.cpp
	PropertiesPanelView.h
	Parse.cpp
	WidgetTypes.cpp
	BasisManager.h
	WidgetContainer.h
	SettingsWindow.h
	SettingsWindow.cpp
	EditorState.cpp
	PanelProperties.cpp
	WidgetsWindow.cpp
	EditorState.h
	EditorToolTip.h
	CMakeLists.txt
	SaveLoadWindow.h
	PanelUserData.cpp
	MetaSolutionWindow.h
	BasisState.h
	InputManager.cpp
	PanelUserData.h
	PanelViewCell.h
	PanelItems.h
	MetaSolutionWindow.cpp
	PropertiesPanelView.cpp
	EditorWidgets.h
	resource.h
	SaveLoadWindow.cpp
	PanelMainProperties.cpp
	BasisManager.cpp
	LayoutEditor.rc
	WidgetsWindow.h
	InputManager.h
	UndoManager.h
	Parse.h
	WidgetTypes.h
	PanelView.h
	PanelItems.cpp
	PanelProperties.h
	EditorToolTip.cpp
	EditorWidgets.cpp
)

# setup MyGUIEngine target
add_executable(layout_editor ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(layout_editor PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(layout_editor MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(layout_editor
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)

