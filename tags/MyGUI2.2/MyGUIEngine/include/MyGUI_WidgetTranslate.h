/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __MYGUI_WIDGET_TRANSLATE_H__
#define __MYGUI_WIDGET_TRANSLATE_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	inline int getWidgetWidth(WidgetPtr _widget, bool _vert)
	{
		return _vert ? _widget->getWidth() : _widget->getHeight();
	}

	inline int getWidgetHeight(WidgetPtr _widget, bool _vert)
	{
		return _vert ? _widget->getHeight() : _widget->getWidth();
	}

	inline int getWidgetLeft(WidgetPtr _widget, bool _vert)
	{
		return _vert ? _widget->getLeft() : _widget->getTop();
	}

	inline int getWidgetTop(WidgetPtr _widget, bool _vert)
	{
		return _vert ? _widget->getTop() : _widget->getLeft();
	}

	inline void setWidgetSize(WidgetPtr _widget, int _width, int _height, bool _vert)
	{
		_vert ? _widget->setSize(_width, _height) : _widget->setSize(_height, _width);
	}

	inline void setWidgetCoord(WidgetPtr _widget, int _left, int _top, int _width, int _height, bool _vert)
	{
		_vert ? _widget->setCoord(_left, _top, _width, _height) : _widget->setCoord(_top, _left, _height, _width);
	}

	inline void convertWidgetCoord(IntCoord & _coord, bool _vert)
	{
		if ( ! _vert ) {
			std::swap(_coord.left, _coord.top);
			std::swap(_coord.width, _coord.height);
		}
	}

} // namespace MyGUI

#endif // __MYGUI_WIDGET_TRANSLATE_H__
