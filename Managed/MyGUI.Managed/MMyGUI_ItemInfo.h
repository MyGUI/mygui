/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Marshaling.h"

MMYGUI_BEGIN_NAMESPACE

//#ifndef MMYGUI_USING_EXTERNAL_TYPE

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

//#endif // MMYGUI_USING_EXTERNAL_TYPE

template <> struct Convert<const MyGUI::ItemInfo&>
{
	typedef ItemInfo Type;
	inline static const ItemInfo& To(const MyGUI::ItemInfo& _value) { return reinterpret_cast<const ItemInfo&>(_value); }
	inline static MyGUI::ItemInfo& From(ItemInfo& _value) { return reinterpret_cast<MyGUI::ItemInfo&>(_value); }
};

MMYGUI_END_NAMESPACE
