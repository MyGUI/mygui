/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Macros.h"
#include "MMyGUI_MouseButton.h"
#include "MMyGUI_KeyCode.h"
#include "MMyGUI_Marshaling.h"

MMYGUI_BEGIN_NAMESPACE

//#ifndef MMYGUI_USING_EXTERNAL_TYPE

public value struct NotifyItemData
{
	enum struct NotifyItem
	{
		MousePressed,
		MouseReleased,
		KeyPressed,
		KeyReleased
	};

	System::UInt32 index;
	NotifyItem notify;
	int x;
	int y;
	MouseButton id;
	KeyCode code;
	System::UInt32 key;
};

//#endif // MMYGUI_USING_EXTERNAL_TYPE

template <> struct Convert<const MyGUI::NotifyItemData&>
{
	typedef NotifyItemData Type;
	inline static const NotifyItemData& To(const MyGUI::NotifyItemData& _value) { return reinterpret_cast<const NotifyItemData&>(_value); }
	inline static MyGUI::NotifyItemData& From(NotifyItemData& _value) { return reinterpret_cast<MyGUI::NotifyItemData&>(_value); }
};

MMYGUI_END_NAMESPACE
