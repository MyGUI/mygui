// this replaces MyGUI_Platform.h, because cppcheck can't parse defines there.
// to use it add this file to cppcheck include directories before(!) other
// include directorieswhile checking something like:
// # cppckeck trunk/MyGUIEngine/src -I trunk/Scripts/cppcheck -I trunk/MyGUIEngine/include --enable=all 2> err.txt

#ifndef __MYGUI_PLATFORM_H__
#define __MYGUI_PLATFORM_H__

#define MYGUI_EXPORT
#define MYGUI_PLATFORM 1
#define MYGUI_COMPILER 1
#define MYGUI_EXPORT_DLL
#define MYGUI_DEBUG_MODE 0

#endif // __MYGUI_PLATFORM_H__

