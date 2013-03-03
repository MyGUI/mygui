/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_LAYOUT_DATA_H__
#define __MYGUI_LAYOUT_DATA_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	struct MYGUI_EXPORT ControllerInfo
	{
		std::string type;
		MapString properties;
	};

	struct MYGUI_EXPORT WidgetInfo
	{
	public:
		WidgetInfo() :
			align(Align::Default),
			style(WidgetStyle::Child),
			positionType(None)
		{
		}

	public:
		std::vector<WidgetInfo> childWidgetsInfo;
		// not map because there might be several properties with same name (for example List items)
		VectorStringPairs properties;
		MapString userStrings;
		std::vector<ControllerInfo> controllers;
		std::string type;
		std::string skin;
		Align align;
		WidgetStyle style;
		std::string name;
		std::string layer;

		enum PositionType { Pixels, Relative, None };
		PositionType positionType;
		IntCoord intCoord;
		FloatCoord floatCoord;
	};

	typedef std::vector<WidgetInfo> VectorWidgetInfo;

} // namespace MyGUI

#endif // __MYGUI_LAYOUT_DATA_H__
