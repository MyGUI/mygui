/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_Align.h"

namespace MyGUI
{

	Align Align::parse(std::string_view _value)
	{
		Align result(Enum(0));
		const MapAlign& map_names = Align::getValueNames();
		std::vector<std::string> vec = utility::split(_value);
		for (const auto& pos : vec)
		{
			auto iter = map_names.find(pos);
			if (iter != map_names.end())
			{
				result |= iter->second;
			}
		}
		return result;
	}

	std::string Align::print() const
	{
		std::string result;

		if (mValue & Left)
		{
			if (mValue & Right)
				result = "HStretch";
			else
				result = "Left";
		}
		else if (mValue & Right)
			result = "Right";
		else
			result = "HCenter";

		if (mValue & Top)
		{
			if (mValue & Bottom)
				result += " VStretch";
			else
				result += " Top";
		}
		else if (mValue & Bottom)
			result += " Bottom";
		else
			result += " VCenter";

		return result;
	}

	const Align::MapAlign& Align::getValueNames()
	{
		static MapAlign map_names;

		if (map_names.empty())
		{
#ifndef MYGUI_DONT_USE_OBSOLETE
			// OBSOLETE
			map_names["ALIGN_HCENTER"] = HCenter;
			map_names["ALIGN_VCENTER"] = VCenter;
			map_names["ALIGN_CENTER"] = Center;
			map_names["ALIGN_LEFT"] = Left;
			map_names["ALIGN_RIGHT"] = Right;
			map_names["ALIGN_HSTRETCH"] = HStretch;
			map_names["ALIGN_TOP"] = Top;
			map_names["ALIGN_BOTTOM"] = Bottom;
			map_names["ALIGN_VSTRETCH"] = VStretch;
			map_names["ALIGN_STRETCH"] = Stretch;
			map_names["ALIGN_DEFAULT"] = Default;
#endif

			MYGUI_REGISTER_VALUE(map_names, HCenter);
			MYGUI_REGISTER_VALUE(map_names, VCenter);
			MYGUI_REGISTER_VALUE(map_names, Center);
			MYGUI_REGISTER_VALUE(map_names, Left);
			MYGUI_REGISTER_VALUE(map_names, Right);
			MYGUI_REGISTER_VALUE(map_names, HStretch);
			MYGUI_REGISTER_VALUE(map_names, Top);
			MYGUI_REGISTER_VALUE(map_names, Bottom);
			MYGUI_REGISTER_VALUE(map_names, VStretch);
			MYGUI_REGISTER_VALUE(map_names, Stretch);
			MYGUI_REGISTER_VALUE(map_names, Default);
		}

		return map_names;
	}

} // namespace MyGUI
