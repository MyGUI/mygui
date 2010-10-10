// this replaces MyGUI_Platform.h, because cppcheck can't parse defines there.
// to use it add this file to cppcheck include directories before(!) other
// include directorieswhile checking something like:
// # cppckeck trunk/MyGUIEngine/src -I trunk/Scripts/cppcheck -I trunk/MyGUIEngine/include --enable=all 2> err.txt

#ifndef __MYGUI_PLATFORM_H__
#define __MYGUI_PLATFORM_H__

// I have no idea where it comes from, but cppcheck check all code with and without this define
#define __STDC_ISO_10646__ 1

#define MYGUI_EXPORT
#define MYGUI_PLATFORM 1
#define MYGUI_COMPILER 1
#define MYGUI_EXPORT_DLL
#define MYGUI_DEBUG_MODE 0
#define namespace //

#define min min
#define max max
#define MYGUI_DONT_USE_OBSOLETE

#endif // __MYGUI_PLATFORM_H__
