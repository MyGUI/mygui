/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_MACROS_H_
#define MYGUI_MACROS_H_

#include "MyGUI_Prerequest.h"
#include <limits>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace MyGUI
{

	constexpr size_t ITEM_NONE = (std::numeric_limits<size_t>::max)();
	constexpr int DEFAULT = -1;
	constexpr float ALPHA_MAX = 1.0f;
	constexpr float ALPHA_MIN = 0.0f;

	constexpr int MYGUI_FLAG_NONE = 0;
	constexpr int MYGUI_FLAG(uint8_t num) { return 1 << num; }

	template <class Map, class Value>
	inline void mapSet(Map& map, std::string_view key, const Value& value)
	{
		auto it = map.find(key);
		if (it == map.end())
			map.emplace(key, value);
		else
			it->second = value;
	}

} // namespace MyGUI

#endif // MYGUI_MACROS_H_
