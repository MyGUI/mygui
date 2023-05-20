/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ChildSkinInfo.h"

namespace MyGUI
{

	ChildSkinInfo::ChildSkinInfo() :
		style(WidgetStyle::Child),
		align(Align::Default)
	{
	}

	ChildSkinInfo::ChildSkinInfo(std::string_view _type, const WidgetStyle& _style, std::string_view _skin, const IntCoord& _coord, const Align& _align, std::string_view _layer, std::string_view _name) :
		type(_type),
		skin(_skin),
		name(_name),
		layer(_layer),
		style(_style),
		coord(_coord),
		align(_align)
	{
		// set Child style by default
		if (style == WidgetStyle::MAX)
			style = WidgetStyle::Child;
	}

	void ChildSkinInfo::addParam(std::string_view _key, std::string_view _value)
	{
		auto it = params.find(_key);
		if (it == params.end())
			params.emplace(_key, _value);
		else
			it->second = _value;
	}

} // namespace MyGUI
