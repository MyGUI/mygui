/*!
	@file
	@author		George Evmenov
	@date		07/2010
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
