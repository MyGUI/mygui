/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_CHILD_SKIN_INFO_H_
#define MYGUI_CHILD_SKIN_INFO_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetStyle.h"

namespace MyGUI
{

	struct MYGUI_EXPORT ChildSkinInfo
	{
	public:
		ChildSkinInfo();
		ChildSkinInfo(const std::string& _type, const WidgetStyle& _style, const std::string& _skin, const IntCoord& _coord, const Align& _align, const std::string& _layer, const std::string& _name);

		void addParam(const std::string& _key, const std::string& _value);

	public:
		std::string type, skin, name, layer;
		WidgetStyle style;
		IntCoord coord;
		Align align;
		MapString params;
	};

	typedef std::vector<ChildSkinInfo> VectorChildSkinInfo;

} // namespace MyGUI


#endif // MYGUI_CHILD_SKIN_INFO_H_
