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
	// ��� ��� C# object
	typedef int* IUnknown;
	// ��� ��� ������ ������� C#
	typedef int* Interface;

	// �������� ������� �� ������
	Interface CreateWrapper(MyGUI::WidgetPtr _widget);
	// ���������� ������, ������������� �������
	MyGUI::WidgetPtr GetNativeByWrapper(Interface _wrapper);
}

#endif // __EXPORT_DEFINE_H__