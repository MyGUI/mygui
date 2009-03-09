INCLUDES = $(STLPORT_CFLAGS)\
	$(FT2_CFLAGS)\
	$(OGRE_CFLAGS)\
	$(OIS_CFLAGS)\
	-I$(top_srcdir)/MyGUIEngine/include\
	-I$(top_srcdir)/Common\
	-I$(top_srcdir)/Common/ItemBox\
	$(CFLAGS)

noinst_PROGRAMS = Demo_Gui

Demo_Gui_SOURCES = \
	ColourWindow.cpp\
	ColourWindowBox.cpp\
	ColourWindowCellView.cpp\
	DemoKeeper.cpp\
	EditorWindow.cpp\
	InformationWindow.cpp\
	MainPanel.cpp\
	$(top_srcdir)/Common/Base/Main.cpp\
	$(top_srcdir)/Common/Base/BaseManager.cpp
Demo_Gui_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
Demo_Gui_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui -luuid
