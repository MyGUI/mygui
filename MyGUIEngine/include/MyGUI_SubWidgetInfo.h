/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_SUB_WIDGET_INFO_H_
#define MYGUI_SUB_WIDGET_INFO_H_

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	// информация, об одном сабвиджете
	struct SubWidgetInfo
	{
	public:
		SubWidgetInfo(const std::string& _type, const IntCoord& _coord, Align _align) :
			coord(_coord),
			align(_align),
			type(_type)
		{
		}

	public:
		IntCoord coord;
		Align align;
		std::string type;
	};

	typedef std::vector<SubWidgetInfo> VectorSubWidgetInfo;

} // namespace MyGUI

#endif // MYGUI_SUB_WIDGET_INFO_H_
