/*!
	@file
	@author		Albert Semenov
	@date		11/2007
*/
#ifndef EXPORT_DEFINE_H_
#define EXPORT_DEFINE_H_

#define MYGUIEXPORT extern "C" __declspec( dllexport )
#define MYGUICALL __cdecl
#define MYGUICALLBACK __stdcall

#include <MyGUI.h>

#endif // EXPORT_DEFINE_H_
