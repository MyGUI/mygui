INCLUDES = $(STLPORT_CFLAGS)\
	$(FT2_CFLAGS)\
	$(OGRE_CFLAGS)\
	$(OIS_CFLAGS)\
	-I$(top_srcdir)/MyGUIEngine/include\
	-I$(top_srcdir)/Common\
	-I/usr/include/agg2\
	$(CFLAGS)

noinst_PROGRAMS = UnitTest_Canvas

UnitTest_Canvas_SOURCES = \
	DemoKeeper.cpp\
	$(top_srcdir)/Common/Base/Main.cpp\
	$(top_srcdir)/Common/RenderBox/RenderBoxWrap.cpp\
	$(top_srcdir)/Common/Base/BaseManager.cpp
UnitTest_Canvas_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
UnitTest_Canvas_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui -luuid -lagg
