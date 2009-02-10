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

//#ifndef MMYGUI_USING_INTERFACE

public value struct ItemDropInfo
{
	MMYGUI_WIDGET_NAME^ sender;
	System::UInt32 sender_index;
	MMYGUI_WIDGET_NAME^ reseiver;
	System::UInt32 reseiver_index;
};

//#endif // MMYGUI_USING_INTERFACE

template <> struct Convert<const MyGUI::ItemDropInfo&>
{
	typedef ItemDropInfo Type;
	inline static ItemDropInfo To(const MyGUI::ItemDropInfo& _value)
	{
		ItemDropInfo info;
		info.sender_index = _value.sender_index;
		info.sender = Convert<MyGUI::Widget*>::To(_value.sender);
		info.reseiver_index = _value.reseiver_index;
		info.reseiver = Convert<MyGUI::Widget*>::To(_value.reseiver);
		return info;
	}
};

MMYGUI_END_NAMESPACE
