INCLUDES = $(STLPORT_CFLAGS)\
	$(FT2_CFLAGS)\
	$(OGRE_CFLAGS)\
	$(OIS_CFLAGS)\
	-I$(top_srcdir)/MyGUIEngine/include\
	-I$(top_srcdir)/Common\
	$(CFLAGS)

noinst_PROGRAMS = Demo_Controllers

Demo_Controllers_SOURCES = \
	DemoKeeper.cpp\
	State.cpp\
	$(top_srcdir)/Common/Base/Main.cpp\
	$(top_srcdir)/Common/Base/BaseManager.cpp
Demo_Controllers_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
Demo_Controllers_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui -luuid
