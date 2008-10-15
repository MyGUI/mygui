INCLUDES = $(STLPORT_CFLAGS)\
	$(FT2_CFLAGS)\
	$(OGRE_CFLAGS)\
	$(OIS_CFLAGS)\
	-I$(top_srcdir)/MyGUIEngine/include\
	-I$(top_srcdir)/Common\
	$(CFLAGS)

noinst_PROGRAMS = Demo_Console

Demo_Console_SOURCES = \
	Main.cpp\
	Console.cpp\
	DemoKeeper.cpp\
	$(top_srcdir)/Common/BaseLayout.cpp\
	$(top_srcdir)/Common/Base/BaseManager.cpp
Demo_Console_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
Demo_Console_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui -luuid
