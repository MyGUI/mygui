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

	public value struct MessageStyle
	{

		MMYGUI_MANAGED_NATIVE_CONVERSIONS_FOR_VALUE(MessageStyle);
		MMYGUI_DECLARE_EQUALS(MessageStyle);

		MMYGUI_DECLARE_ENUM(MessageStyle, None);
		MMYGUI_DECLARE_ENUM(MessageStyle, Ok);
		MMYGUI_DECLARE_ENUM(MessageStyle, Yes);
		MMYGUI_DECLARE_ENUM(MessageStyle, No);
		MMYGUI_DECLARE_ENUM(MessageStyle, Abort);
		MMYGUI_DECLARE_ENUM(MessageStyle, Retry);
		MMYGUI_DECLARE_ENUM(MessageStyle, Ignore);
		MMYGUI_DECLARE_ENUM(MessageStyle, Cancel);
		MMYGUI_DECLARE_ENUM(MessageStyle, Try);
		MMYGUI_DECLARE_ENUM(MessageStyle, Continue);

		MMYGUI_DECLARE_ENUM(MessageStyle, Button1);
		MMYGUI_DECLARE_ENUM(MessageStyle, Button2);
		MMYGUI_DECLARE_ENUM(MessageStyle, Button3);
		MMYGUI_DECLARE_ENUM(MessageStyle, Button4);

		MMYGUI_DECLARE_ENUM(MessageStyle, IconDefault);
		MMYGUI_DECLARE_ENUM(MessageStyle, IconInfo);
		MMYGUI_DECLARE_ENUM(MessageStyle, IconQuest);
		MMYGUI_DECLARE_ENUM(MessageStyle, IconError);
		MMYGUI_DECLARE_ENUM(MessageStyle, IconWarning);

		MMYGUI_DECLARE_ENUM(MessageStyle, Icon1);
		MMYGUI_DECLARE_ENUM(MessageStyle, Icon2);
		MMYGUI_DECLARE_ENUM(MessageStyle, Icon3);
		MMYGUI_DECLARE_ENUM(MessageStyle, Icon4);
		MMYGUI_DECLARE_ENUM(MessageStyle, Icon5);
		MMYGUI_DECLARE_ENUM(MessageStyle, Icon6);
		MMYGUI_DECLARE_ENUM(MessageStyle, Icon7);
		MMYGUI_DECLARE_ENUM(MessageStyle, Icon8);

		explicit MessageStyle( int _value ) : value( _value) { }

		static MessageStyle operator | ( MessageStyle lvalue, MessageStyle rvalue ) { return MessageStyle( lvalue.value | rvalue.value ); }

		static bool operator == ( MessageStyle lvalue, MessageStyle rvalue ) { return ( lvalue.value == rvalue.value ); }

	private:
		int value;
	};

} // namespace MMyGUI
