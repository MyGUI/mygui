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

MMYGUI_BEGIN_NAMESPACE

//#ifndef MMYGUI_USING_INTERFACE

public enum struct MessageBoxStyle
{
	None = MyGUI::MessageStyle::None,
	Ok = MyGUI::MessageStyle::Ok,
	Yes = MyGUI::MessageStyle::Yes,
	No = MyGUI::MessageStyle::No,
	Abort = MyGUI::MessageStyle::Abort,
	Retry = MyGUI::MessageStyle::Retry,
	Ignore = MyGUI::MessageStyle::Ignore,
	Cancel = MyGUI::MessageStyle::Cancel,
	Try = MyGUI::MessageStyle::Try,
	Continue = MyGUI::MessageStyle::Continue,

	Button1 = MyGUI::MessageStyle::Button1,
	Button2 = MyGUI::MessageStyle::Button2,
	Button3 = MyGUI::MessageStyle::Button3,
	Button4 = MyGUI::MessageStyle::Button4,

	IconDefault = MyGUI::MessageStyle::IconDefault,
	IconInfo = MyGUI::MessageStyle::IconInfo,
	IconQuest = MyGUI::MessageStyle::IconQuest,
	IconError = MyGUI::MessageStyle::IconError,
	IconWarning = MyGUI::MessageStyle::IconWarning,

	Icon1 = MyGUI::MessageStyle::Icon1,
	Icon2 = MyGUI::MessageStyle::Icon2,
	Icon3 = MyGUI::MessageStyle::Icon3,
	Icon4 = MyGUI::MessageStyle::Icon4,
	Icon5 = MyGUI::MessageStyle::Icon5,
	Icon6 = MyGUI::MessageStyle::Icon6,
	Icon7 = MyGUI::MessageStyle::Icon7,
	Icon8 = MyGUI::MessageStyle::Icon8
};

//#endif // MMYGUI_USING_INTERFACE

template <> struct Convert<MyGUI::MessageStyle>
{
	typedef MessageBoxStyle Type;
	inline static const MessageBoxStyle& To(const MyGUI::MessageStyle& _value) { return reinterpret_cast<const MessageBoxStyle&>(_value); }
	inline static MyGUI::MessageStyle& From(MessageBoxStyle& _value) { return reinterpret_cast<MyGUI::MessageStyle&>(_value); }
};

MMYGUI_END_NAMESPACE
