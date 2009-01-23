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
	template <typename T> T* castWidget(Widget * _widget)
	{
		MYGUI_DEBUG_ASSERT(null != _widget, "Error static cast, widget == null");
		return _widget->castType<T>();
	}

} // namespace MyGUI

#endif // __MYGUI_CAST_WIDGET_H__
