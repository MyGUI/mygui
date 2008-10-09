INCLUDES = $(STLPORT_CFLAGS)\
	$(FT2_CFLAGS)\
	$(OGRE_CFLAGS)\
	$(OIS_CFLAGS)\
	-I$(top_srcdir)/MyGUIEngine/include\
	-I$(top_srcdir)/Common\
	-I$(top_srcdir)/Common/PanelView\
	$(CFLAGS)

noinst_PROGRAMS = Demo_PanelView

Demo_PanelView_SOURCES = \
	BasisManager.cpp\
	DemoKeeper.cpp\
	PanelDirector.cpp\
	PanelDynamic.cpp\
	PanelStatic.cpp\
	$(top_srcdir)/Common/BaseLayout.cpp\
	$(top_srcdir)/Common/Base/BaseManager.cpp\
	$(top_srcdir)/Common/PanelView/PanelCell.cpp\
	$(top_srcdir)/Common/PanelView/PanelView.cpp\
	$(top_srcdir)/Common/PanelView/PanelViewWindow.cpp
Demo_PanelView_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
Demo_PanelView_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui -luuid
