INCLUDES = $(STLPORT_CFLAGS)\
	$(FT2_CFLAGS)\
	$(OGRE_CFLAGS)\
	$(OIS_CFLAGS)\
	-I$(top_srcdir)/MyGUIEngine/include\
	-I$(top_srcdir)/Common\
	$(CFLAGS)

install-exec-local:
	$(INSTALL_PROGRAM) Demo_Font $(top_builddir)/Shared/linux

noinst_PROGRAMS = Demo_Font

Demo_Font_SOURCES = \
	DemoKeeper.cpp\
	FontPanel.cpp\
	FontView.cpp\
	TextureView.cpp\
	$(top_srcdir)/Common/Base/Main.cpp\
	$(top_srcdir)/Common/Base/BaseManager.cpp
Demo_Font_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
Demo_Font_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui -luuid
