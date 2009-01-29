INCLUDES = $(STLPORT_CFLAGS)\
	$(FT2_CFLAGS)\
	$(OGRE_CFLAGS)\
	$(OIS_CFLAGS)\
	-I$(top_srcdir)/MyGUIEngine/include\
	-I$(top_srcdir)/Common\
	$(CFLAGS)

noinst_PROGRAMS = LayoutEditor

LayoutEditor_SOURCES = \
	BasisManager.cpp\
	EditorState.cpp\
	EditorToolTip.cpp\
	EditorWidgets.cpp\
	InputManager.cpp\
	PanelItems.cpp\
	PanelMainProperties.cpp\
	PanelProperties.cpp\
	PanelUserData.cpp\
	Parse.cpp\
	PropertiesPanelView.cpp\
	SaveLoadWindow.cpp\
	SettingsWindow.cpp\
	UndoManager.cpp\
	WidgetsWindow.cpp\
	WidgetTypes.cpp
LayoutEditor_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
LayoutEditor_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui -luuid
