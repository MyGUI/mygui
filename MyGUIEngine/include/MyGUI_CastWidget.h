/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_CAST_WIDGET_H__
#define __MYGUI_CAST_WIDGET_H__

#include "MyGUI_Widget.h"

namespace MyGUI
{
	// шаблонный класс для проверки типа виджета
	template <class T> inline T* castWidget(Widget * _widget)
	{
		MYGUI_DEBUG_ASSERT(null != _widget, "Error static cast, widget == null");
		MYGUI_DEBUG_ASSERT(null != dynamic_cast<T*>(_widget),
			"Error dynamic cast : dest type = '" << T::_getType() 
			<< "' source name = '" << _widget->getName() 
			<< "' source type = '" << _widget->getWidgetType() << "'");
		return static_cast<T*>(_widget);
	}

} // namespace MyGUI

#endif // __MYGUI_CAST_WIDGET_H__
