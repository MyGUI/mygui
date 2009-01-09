/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_TYPES_H__
#define __MYGUI_TYPES_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Utility.h"
#include "MyGUI_TPoint.h"
#include "MyGUI_TSize.h"
#include "MyGUI_TRect.h"
#include "MyGUI_TCoord.h"

namespace MyGUI
{

	// определяем типы
	typedef types::TPoint<int> IntPoint;
	typedef types::TPoint<float> FloatPoint;

	typedef types::TSize<int> IntSize;
	typedef types::TSize<float> FloatSize;

	typedef types::TRect<int> IntRect;
	typedef types::TRect<float> FloatRect;

	typedef types::TCoord<int> IntCoord;
	typedef types::TCoord<float> FloatCoord;

	typedef std::map<std::string, std::string> MapString;
	typedef std::vector<std::string> VectorString;

	typedef char int8;
	typedef short int16;
	typedef int int32;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned int uint;

	typedef unsigned int Char;
	typedef std::string UString;

	// тип, для двойного преобразования
	template <typename Type>
	struct FakeType
	{
		FakeType(Type _value) : value(_value) { }
		operator Type () { return value; }
		Type value;
	};

} // namespace MyGUI

#endif // __MYGUI_TYPES_H__
