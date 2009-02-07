/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Macros.h"
#include "MMyGUI_Marshaling.h"

namespace MMyGUI
{

	public enum struct MenuItemType
	{
		Normal = MyGUI::MenuItemType::Normal,
		Popup = MyGUI::MenuItemType::Popup,
		Separator = MyGUI::MenuItemType::Separator
	};

	template <> struct Convert<MyGUI::MenuItemType>
	{
		typedef MenuItemType Type;
		inline static const MenuItemType& To(const MyGUI::MenuItemType& _value) { return reinterpret_cast<const MenuItemType&>(_value); }
		inline static MyGUI::MenuItemType& From(MenuItemType& _value) { return reinterpret_cast<MyGUI::MenuItemType&>(_value); }
	};

} // namespace MMyGUI
