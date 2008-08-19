INCLUDES = $(STLPORT_CFLAGS) $(FT2_CFLAGS) $(OGRE_CFLAGS) $(OIS_CFLAGS) -I$(top_srcdir)/MyGUIEngine/include $(CFLAGS) -I$(top_srcdir)/Common

noinst_PROGRAMS = Demo_Console

Demo_Console_SOURCES = BasisManager.cpp Console.cpp DemoKeeper.cpp $(top_srcdir)/Common/BaseLayout.cpp
Demo_Console_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
Demo_Console_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui
