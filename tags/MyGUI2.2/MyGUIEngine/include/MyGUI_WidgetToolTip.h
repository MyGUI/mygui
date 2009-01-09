/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __MYGUI_WIDGET_TOOLTIP_H__
#define __MYGUI_WIDGET_TOOLTIP_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_WidgetToolTip.h"

namespace MyGUI
{

	/** Info about tootip state */
	struct ToolTipInfo
	{
		enum ToolTipType
		{
			Show,
			Hide
		};

		ToolTipInfo(ToolTipType _type) :
			type(_type), index(ITEM_NONE) { }

		ToolTipInfo(ToolTipType _type, size_t _index, const IntPoint & _point) :
			type(_type), index(_index), point(_point) { }

		ToolTipType type;
		size_t index;
		IntPoint point;
	};

} // namespace MyGUI

#endif //__MYGUI_WIDGET_TOOLTIP_H__
