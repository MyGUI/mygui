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

	int getWidgetWidth(WidgetPtr _widget, bool _vert)
	{
		return _vert ? _widget->getWidth() : _widget->getHeight();
	}

	int getWidgetHeight(WidgetPtr _widget, bool _vert)
	{
		return _vert ? _widget->getHeight() : _widget->getWidth();
	}

	int getWidgetLeft(WidgetPtr _widget, bool _vert)
	{
		return _vert ? _widget->getLeft() : _widget->getTop();
	}

	int getWidgetTop(WidgetPtr _widget, bool _vert)
	{
		return _vert ? _widget->getTop() : _widget->getLeft();
	}

	void setWidgetSize(WidgetPtr _widget, int _width, int _height, bool _vert)
	{
		_vert ? _widget->setSize(_width, _height) : _widget->setSize(_height, _width);
	}

	void setWidgetPosition(WidgetPtr _widget, int _left, int _top, int _width, int _height, bool _vert)
	{
		_vert ? _widget->setPosition(_left, _top, _width, _height) : _widget->setPosition(_top, _left, _height, _width);
	}

	void convertWidgetCoord(IntCoord & _coord, bool _vert)
	{
		if ( ! _vert ) {
			std::swap(_coord.left, _coord.top);
			std::swap(_coord.width, _coord.height);
		}
	}

} // namespace MyGUI

#endif // __MYGUI_WIDGET_TRANSLATE_H__
