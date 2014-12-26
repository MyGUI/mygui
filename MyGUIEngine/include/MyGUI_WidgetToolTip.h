/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_WIDGET_TOOLTIP_H_
#define MYGUI_WIDGET_TOOLTIP_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	/** Info about tootip state */
	struct ToolTipInfo
	{
		enum ToolTipType
		{
			Hide,
			Show,
			Move
		};

		ToolTipInfo(ToolTipType _type) :
			type(_type),
			index(ITEM_NONE)
		{
		}

		ToolTipInfo(ToolTipType _type, size_t _index, const IntPoint& _point) :
			type(_type),
			index(_index),
			point(_point)
		{
		}

		ToolTipType type;
		size_t index;
		IntPoint point;
	};

} // namespace MyGUI

#endif //__MYGUI_WIDGET_TOOLTIP_H__
