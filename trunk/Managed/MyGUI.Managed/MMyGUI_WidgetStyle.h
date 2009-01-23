/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Macros.h"

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
