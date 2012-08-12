/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/

#ifndef _3dd2ef40_ae18_44a3_aa18_5909c0c05ea2_
#define _3dd2ef40_ae18_44a3_aa18_5909c0c05ea2_

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

#endif
