INCLUDES = $(STLPORT_CFLAGS)\
	$(FT2_CFLAGS)\
	$(OGRE_CFLAGS)\
	$(OIS_CFLAGS)\
	-I$(top_srcdir)/MyGUIEngine/include\
	-I$(top_srcdir)/Common\
	$(CFLAGS)

noinst_PROGRAMS = Demo_Gui

Demo_Gui_SOURCES = \
	DemoKeeper.cpp\
	Main.cpp\
	$(top_srcdir)/Common/BaseLayout.cpp\
	$(top_srcdir)/Common/Base/BaseManager.cpp
Demo_Gui_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
Demo_Gui_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui -luuid
