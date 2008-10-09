INCLUDES = $(STLPORT_CFLAGS) $(FT2_CFLAGS) $(OGRE_CFLAGS) $(OIS_CFLAGS) -I$(top_srcdir)/MyGUIEngine/include $(CFLAGS) -I$(top_srcdir)/Common

noinst_PROGRAMS = LayoutEditor

LayoutEditor_SOURCES = \
	BasisManager.cpp\
	EditorState.cpp\
	EditorToolTip.cpp\
	InputManager.cpp\
	PanelItems.cpp\
	PanelMainProperties.cpp\
	PanelProperties.cpp\
	PanelUserData.cpp\
	PropertiesPanelView.cpp\
	SettingsWindow.cpp\
	UndoManager.cpp\
	WidgetContainer.cpp\
	WidgetTypes.cpp\
	WidgetsWindow.cpp\
	$(top_srcdir)/Common/BaseLayout.cpp\
	$(top_srcdir)/Common/PanelView/PanelCell.cpp\
	$(top_srcdir)/Common/PanelView/PanelView.cpp\
	$(top_srcdir)/Common/PanelView/PanelViewWindow.cpp
LayoutEditor_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
LayoutEditor_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui -luuid
