/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_TYPES_H__
#define __MYGUI_TYPES_H__

#include "MyGUI_Prerequest.h"

#include <vector>
#include <map>
#include <string>

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
	typedef std::pair<std::string, std::string> PairString;
	typedef std::vector<PairString> VectorStringPairs;

	typedef char int8;
	typedef short int16;
	typedef int int32;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned int uint;

	typedef unsigned int Char;

} // namespace MyGUI

#endif // __MYGUI_TYPES_H__
