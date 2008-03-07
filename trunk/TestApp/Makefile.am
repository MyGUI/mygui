INCLUDES = $(STLPORT_CFLAGS) $(FT2_CFLAGS) $(OGRE_CFLAGS) $(OIS_CFLAGS) -I$(top_srcdir)/MyGUIEngine/include $(CFLAGS)

noinst_PROGRAMS = TestApp

TestApp_SOURCES = BasisManager.cpp DemoKeeper.cpp
TestApp_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
TestApp_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui
