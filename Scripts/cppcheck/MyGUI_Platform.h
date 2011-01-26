// this replaces MyGUI_Platform.h, because cppcheck can't parse defines there.
// to use it add this file to cppcheck include directories before(!) other
// include directories while checking something like:
// # cppcheck trunk/MyGUIEngine/src -I trunk/Scripts/cppcheck -I trunk/MyGUIEngine/include --enable=all

#ifndef __MYGUI_PLATFORM_H__
#define __MYGUI_PLATFORM_H__

#ifdef DISABLE_NAMESPACES
	#define namespace //
#endif

#define MYGUI_EXPORT
#define MYGUI_PLATFORM 1
#define MYGUI_COMPILER 2
#define MYGUI_EXPORT_DLL
#define MYGUI_DEBUG_MODE 1

#define min min
#define max max
#define MYGUI_DONT_USE_OBSOLETE

#endif // __MYGUI_PLATFORM_H__
