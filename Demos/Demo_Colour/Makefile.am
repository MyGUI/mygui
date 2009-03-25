INCLUDES = $(STLPORT_CFLAGS)\
	$(FT2_CFLAGS)\
	$(OGRE_CFLAGS)\
	$(OIS_CFLAGS)\
	-I$(top_srcdir)/MyGUIEngine/include\
	-I$(top_srcdir)/Common\
	$(CFLAGS)

install-exec-local:
	$(INSTALL_PROGRAM) Demo_Colour $(top_builddir)/Shared/linux

noinst_PROGRAMS = Demo_Colour

Demo_Colour_SOURCES = \
	ColourPanel.cpp\
	DemoKeeper.cpp\
	EditPanel.cpp\
	$(top_srcdir)/Common/Base/Main.cpp\
	$(top_srcdir)/Common/Base/BaseManager.cpp
Demo_Colour_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
Demo_Colour_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui -luuid
