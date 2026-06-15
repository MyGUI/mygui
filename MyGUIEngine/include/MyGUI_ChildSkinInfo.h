/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_CHILD_SKIN_INFO_H_
#define MYGUI_CHILD_SKIN_INFO_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetStyle.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	struct MYGUI_EXPORT ChildSkinInfo
	{
	public:
		ChildSkinInfo();
		ChildSkinInfo(
			std::string_view _type,
			const WidgetStyle& _style,
			std::string_view _skin,
			const IntCoord& _coord,
			const Align& _align,
			std::string_view _layer,
			std::string_view _name);

		void addParam(std::string_view _key, std::string_view _value);

	public:
		std::string type, skin, name, layer;
		WidgetStyle style;
		IntCoord coord;
		Align align;
		MapString params;
	};

	using VectorChildSkinInfo = std::vector<ChildSkinInfo>;

} // namespace MyGUI


#endif // MYGUI_CHILD_SKIN_INFO_H_
