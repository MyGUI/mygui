INCLUDES = $(STLPORT_CFLAGS) $(FT2_CFLAGS) $(OGRE_CFLAGS) $(OIS_CFLAGS) -I$(top_srcdir)/MyGUIEngine/include $(CFLAGS) -I$(top_srcdir)/Common

noinst_PROGRAMS = Demo_ItemBox

Demo_ItemBox_SOURCES = BaseItemBox.cpp BasisManager.cpp CellView.cpp DemoKeeper.cpp ItemBoxH.cpp ItemBoxV.cpp ToolTip.cpp $(top_srcdir)/Common/BaseLayout.cpp
Demo_ItemBox_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
Demo_ItemBox_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui
