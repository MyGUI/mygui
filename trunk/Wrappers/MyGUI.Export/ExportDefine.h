/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __EXPORT_DEFINE_H__
#define __EXPORT_DEFINE_H__

#define MYGUIEXPORT extern "C" __declspec( dllexport ) 
#define MYGUICALL __cdecl 
#define MYGUICALLBACK __stdcall

#include <MyGUI.h>

namespace Export
{
	// тип для C# object
	typedef int* IUnknown;
	// тип для любого объекта C#
	typedef int* Interface;

	// создание обертки на виджет
	Interface CreateWrapper(MyGUI::WidgetPtr _widget);
	// возвращает виджет, принадлежащий обертке
	MyGUI::WidgetPtr GetNativeByWrapper(Interface _wrapper);
}

#endif // __EXPORT_DEFINE_H__