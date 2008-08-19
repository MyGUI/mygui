#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.50)
AC_INIT( [MYGUI], 2.2.0 )

AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE( [MYGUI], 2.2.0 )

CFLAGS="$CFLAGS"
CXXFLAGS="$CXXFLAGS"
LIBS="$LIBS"

dnl Check for programs
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_DISABLE_STATIC
AM_PROG_LIBTOOL

dnl Other stuff we need
MYGUI_USE_STLPORT
AC_CHECK_FT2(9.0.0)
MYGUI_CHECK_OGRE
MYGUI_CHECK_OIS

AC_CONFIG_FILES([Makefile
                 MyGUIEngine/Makefile
                 MyGUIEngine/src/Makefile
                 MyGUIEngine/include/Makefile
                 TestApp/Makefile
                 LayoutEditor/Makefile
                 Demos/Makefile
                 Demos/Demo_Console/Makefile
                 Demos/Demo_PluginStrangeButton/Makefile
                 Demos/Demo_StaticImage/Makefile
                 Demos/Demo_ItemBox/Makefile
                 Demos/Demo_ScrollView/Makefile
                 MyGUI.pc])
AC_OUTPUT
