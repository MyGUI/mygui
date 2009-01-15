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

	public value struct MenuItemType
	{

		MMYGUI_MANAGED_NATIVE_CONVERSIONS_FOR_VALUE(MenuItemType);
		MMYGUI_DECLARE_EQUALS(MenuItemType);

		MMYGUI_DECLARE_ENUM(MenuItemType, Normal);
		MMYGUI_DECLARE_ENUM(MenuItemType, Popup);
		MMYGUI_DECLARE_ENUM(MenuItemType, Separator);

		explicit MenuItemType( int _value ) : value( _value) { }

		static bool operator == ( MenuItemType lvalue, MenuItemType rvalue ) { return ( lvalue.value == rvalue.value ); }

	private:
		int value;
	};

} // namespace MMyGUI
