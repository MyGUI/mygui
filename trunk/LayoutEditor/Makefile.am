INCLUDES = $(STLPORT_CFLAGS) $(FT2_CFLAGS) $(OGRE_CFLAGS) $(OIS_CFLAGS) -I$(top_srcdir)/MyGUIEngine/include $(CFLAGS)

noinst_PROGRAMS = LayoutEditor

LayoutEditor_SOURCES = BasisManager.cpp EditorState.cpp UndoManager.cpp WidgetContainer.cpp WidgetTypes.cpp
LayoutEditor_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
LayoutEditor_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui
