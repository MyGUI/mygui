INCLUDES = $(STLPORT_CFLAGS) $(FT2_CFLAGS) $(OGRE_CFLAGS) $(OIS_CFLAGS) -I$(top_srcdir)/MyGUIEngine/include $(CFLAGS) -I$(top_srcdir)/Common

noinst_PROGRAMS = LayoutEditor

LayoutEditor_SOURCES = BasisManager.cpp EditorState.cpp InputManager.cpp UndoManager.cpp WidgetContainer.cpp WidgetTypes.cpp EditorToolTip.cpp $(top_srcdir)/Common/BaseLayout.cpp
LayoutEditor_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
LayoutEditor_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui
