/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_CAST_WIDGET_H__
#define __MYGUI_CAST_WIDGET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{
	// шаблонный класс для проверки типа виджета
	template <class T> inline T* castWidget(Widget * _widget)
	{
		MYGUI_DEBUG_ASSERT(null != _widget, "Error static cast, widget == null");

#if MYGUI_DEBUG_MODE == 1
		T * widget = dynamic_cast<T*>(_widget);
		if (widget == null) {
			MYGUI_EXCEPT("Error dynamic cast : dest type = '" << T::_getType() 
				<< "' source name = '" << _widget->getName() 
				<< "' source type = '" << _widget->getWidgetType() << "'");
		}
		return widget;
#else
		return static_cast<T*>(_widget);
#endif
	}

} // namespace MyGUI

#endif // __MYGUI_CAST_WIDGET_H__
