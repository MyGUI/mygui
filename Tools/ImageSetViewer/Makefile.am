INCLUDES = $(STLPORT_CFLAGS)\
	$(FT2_CFLAGS)\
	$(OGRE_CFLAGS)\
	$(OIS_CFLAGS)\
	-I$(top_srcdir)/MyGUIEngine/include\
	-I$(top_srcdir)/Common\
	$(CFLAGS)

noinst_PROGRAMS = ImageSetViewer

ImageSetViewer_SOURCES = \
	Controller.cpp\
	Main.cpp\
	View.cpp\
	$(top_srcdir)/Common/Base/BaseManager.cpp
ImageSetViewer_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
ImageSetViewer_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui -luuid
