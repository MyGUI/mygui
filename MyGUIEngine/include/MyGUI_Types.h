/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TYPES_H_
#define MYGUI_TYPES_H_

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
	using IntPoint = types::TPoint<int>;
	using FloatPoint = types::TPoint<float>;

	using IntSize = types::TSize<int>;
	using FloatSize = types::TSize<float>;

	using IntRect = types::TRect<int>;
	using FloatRect = types::TRect<float>;

	using IntCoord = types::TCoord<int>;
	using FloatCoord = types::TCoord<float>;
	using DoubleCoord = types::TCoord<double>;

	typedef std::map<std::string, std::string> MapString;
	using VectorString = std::vector<std::string>;
	typedef std::pair<std::string, std::string> PairString;
	using VectorStringPairs = std::vector<PairString>;

	using uint8 = unsigned char;
	using uint16 = unsigned short;
	using uint32 = unsigned int;

	using Char = unsigned int;

} // namespace MyGUI

#endif // MYGUI_TYPES_H_
