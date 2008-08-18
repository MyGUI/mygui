INCLUDES = $(STLPORT_CFLAGS) $(FT2_CFLAGS) $(OGRE_CFLAGS) $(OIS_CFLAGS) -I$(top_srcdir)/MyGUIEngine/include $(CFLAGS)

noinst_PROGRAMS = Demo_ScrollView

Demo_PluginStrangeButton_SOURCES = BasisManager.cpp DemoKeeper.cpp
Demo_PluginStrangeButton_LDFLAGS = -L$(top_builddir)/MyGUIEngine/src
Demo_PluginStrangeButton_LDADD = $(STLPORT_LIBS) $(FT2_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lmygui
