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
	typedef unsigned __int8 uint8;
	typedef unsigned __int16 uint16;
	typedef unsigned __int32 uint32;
#elif MYGUI_COMPILER == MYGUI_COMPILER_GNUC
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
#else
	#include <stdint.h>
	typedef unsigned int8_t uint8;
	typedef unsigned int16_t uint16;
	typedef unsigned int32_t uint32;
#endif

	typedef unsigned int uint;
#ifndef byte
	typedef uint8 byte;
#endif
	typedef uint32 Char;

} // namespace MyGUI

#endif // __MYGUI_TYPES_H__
