INCLUDES = $(STLPORT_CFLAGS)\
	$(FT2_CFLAGS)\
	$(OGRE_CFLAGS)\
	$(OIS_CFLAGS)\
	-I$(top_srcdir)/MyGUIEngine/include\
	-I$(top_srcdir)/Common\
	$(CFLAGS)

noinst_PROGRAMS = TestApp

TestApp_SOURCES = \
	DemoKeeper.cpp\
	$(top_srcdir)/Common/Base/Main.cpp\
	$(top_srcdir)/Common/Base/BaseManager.cpp
TestApp_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
TestApp_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui -luuid
