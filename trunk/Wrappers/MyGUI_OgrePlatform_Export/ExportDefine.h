/*!
	@file
	@author		Albert Semenov
	@date		02/2010
*/
#ifndef __EXPORT_DEFINE_H__
#define __EXPORT_DEFINE_H__

#define MYGUIEXPORT extern "C" __declspec( dllexport )
#define MYGUICALL __cdecl
#define MYGUICALLBACK __stdcall

#include <MyGUI.h>

#endif // __EXPORT_DEFINE_H__
