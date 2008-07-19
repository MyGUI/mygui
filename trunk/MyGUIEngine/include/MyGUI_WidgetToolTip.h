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

	enum ToolTipType
	{
		TOOLTIP_SHOW,
		TOOLTIP_HIDE
	};

	struct ToolTipInfo
	{
		ToolTipInfo(ToolTipType _type, size_t _index, IntPoint _point, void * _data) :
			type(_type), index(_index), point(_point), data(_data) { }

		ToolTipInfo(ToolTipType _type) :
			type(_type), index(ITEM_NONE), point(), data(0) { }

		ToolTipType type;
		size_t index;
		IntPoint point;
		void * data;
	};

} // namespace MyGUI

#endif //__MYGUI_WIDGET_TOOLTIP_H__
