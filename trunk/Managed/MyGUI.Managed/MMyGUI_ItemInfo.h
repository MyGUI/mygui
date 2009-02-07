/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Marshaling.h"

namespace MMyGUI
{

	public value struct ItemInfo
	{
		System::UInt32 index;
		bool update;
		bool select;
		bool active;
		bool drag;
		bool drag_accept;
		bool drag_refuse;
	};

	template <> struct Convert<const MyGUI::ItemInfo&>
	{
		typedef ItemInfo Type;
		inline static const ItemInfo& To(const MyGUI::ItemInfo& _value) { return reinterpret_cast<const ItemInfo&>(_value); }
		inline static MyGUI::ItemInfo& From(ItemInfo& _value) { return reinterpret_cast<MyGUI::ItemInfo&>(_value); }
	};

} // namespace MMyGUI
