/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_WIDGET_STYLE_H__
#define __MMYGUI_WIDGET_STYLE_H__

#include <MyGUI.h>
#include "MMyGUI_Common.h"

namespace MMyGUI
{

	public value struct WidgetStyle
	{

		MMYGUI_MANAGED_NATIVE_CONVERSIONS_FOR_VALUE(WidgetStyle);
		MMYGUI_DECLARE_EQUALS(WidgetStyle);

		MMYGUI_DECLARE_ENUM(WidgetStyle, Overlapped);
		MMYGUI_DECLARE_ENUM(WidgetStyle, Child);
		MMYGUI_DECLARE_ENUM(WidgetStyle, Popup);

		explicit WidgetStyle( int _value ) : value( _value) { }

		static bool operator == ( WidgetStyle lvalue, WidgetStyle rvalue ) { return ( lvalue.value == rvalue.value ); }

	private:
		int value;
	};

} // namespace MMyGUI

#endif // __MMYGUI_WIDGET_STYLE_H__
