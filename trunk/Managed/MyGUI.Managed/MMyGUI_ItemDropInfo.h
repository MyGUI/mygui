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
#include "MMyGUI_MarshalingWidget.h"

MMYGUI_BEGIN_NAMESPACE

//#ifndef MMYGUI_USING_EXTERNAL_TYPE

public value struct DDItemInfo
{
	MMYGUI_WIDGET_NAME^ sender;
	System::UInt32 sender_index;
	MMYGUI_WIDGET_NAME^ receiver;
	System::UInt32 receiver_index;
};

//#endif // MMYGUI_USING_EXTERNAL_TYPE

template <> struct Convert<const MyGUI::DDItemInfo&>
{
	typedef DDItemInfo Type;
	inline static DDItemInfo To(const MyGUI::DDItemInfo& _value)
	{
		DDItemInfo info;
		info.sender_index = (System::UInt32)_value.sender_index;
		info.sender = Convert<MyGUI::Widget*>::To(_value.sender);
		info.receiver_index = (System::UInt32)_value.receiver_index;
		info.receiver = Convert<MyGUI::Widget*>::To(_value.receiver);
		return info;
	}
};

MMYGUI_END_NAMESPACE
