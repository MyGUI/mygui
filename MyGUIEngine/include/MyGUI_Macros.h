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
	constexpr float ALPHA_MAX = 1.0f;
	constexpr float ALPHA_MIN = 0.0f;

	constexpr unsigned int MYGUI_FLAG_NONE = 0;
	constexpr unsigned int MYGUI_FLAG(uint8_t num)
	{
		return 1U << num;
	}

	/**
	   Analagous to std::map's operator[], i.e. it replicates map[key] = value.
	   The operator isn't overloaded to accept a heterogenous key like find or emplace,
	   meaning key needs to be of the map's actual key type, rendering it unusable in combination with string_view.
	   This function prevents a (key) string allocation if the key is already present in the map.
	*/
	template<class Map, class Value>
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
