/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_TYPES_H__
#define __MYGUI_TYPES_H__

#include "MyGUI_Prerequest.h"

#include <vector>
#include <map>
#include <string>

#include "MyGUI_Utility.h"
#include "MyGUI_Align.h"
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

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
	#if (_MSC_VER < 1300)
		typedef signed char int8_t;
		typedef signed short int16_t;
		typedef signed int int32_t;
		typedef unsigned char uint8_t;
		typedef unsigned short uint16_t;
		typedef unsigned int uint32_t;
	#else
		typedef signed __int8 int8_t;
		typedef signed __int16 int16_t;
		typedef signed __int32 int32_t;
		typedef unsigned __int8 uint8_t;
		typedef unsigned __int16 uint16_t;
		typedef unsigned __int32 uint32_t;
	#endif
	typedef signed __int64 int64_t;
	typedef unsigned __int64 uint64_t;
#else
	#include <stdint.h>
#endif

	typedef unsigned int uint;
	typedef uint32_t Char;

} // namespace MyGUI

#endif // __MYGUI_TYPES_H__
