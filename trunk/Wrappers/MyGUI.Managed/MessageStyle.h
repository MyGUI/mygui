/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "Config.h"
#include "Marshaling.h"

namespace MyGUI
{
	namespace Managed
	{

		//#ifndef MMYGUI_USING_EXTERNAL_TYPE

		public enum struct MessageBoxStyle
		{
			None = MyGUI::MessageBoxStyle::None,
			Ok = MyGUI::MessageBoxStyle::Ok,
			Yes = MyGUI::MessageBoxStyle::Yes,
			No = MyGUI::MessageBoxStyle::No,
			Abort = MyGUI::MessageBoxStyle::Abort,
			Retry = MyGUI::MessageBoxStyle::Retry,
			Ignore = MyGUI::MessageBoxStyle::Ignore,
			Cancel = MyGUI::MessageBoxStyle::Cancel,
			Try = MyGUI::MessageBoxStyle::Try,
			Continue = MyGUI::MessageBoxStyle::Continue,

			Button1 = MyGUI::MessageBoxStyle::Button1,
			Button2 = MyGUI::MessageBoxStyle::Button2,
			Button3 = MyGUI::MessageBoxStyle::Button3,
			Button4 = MyGUI::MessageBoxStyle::Button4,

			IconDefault = MyGUI::MessageBoxStyle::IconDefault,
			IconInfo = MyGUI::MessageBoxStyle::IconInfo,
			IconQuest = MyGUI::MessageBoxStyle::IconQuest,
			IconError = MyGUI::MessageBoxStyle::IconError,
			IconWarning = MyGUI::MessageBoxStyle::IconWarning,

			Icon1 = MyGUI::MessageBoxStyle::Icon1,
			Icon2 = MyGUI::MessageBoxStyle::Icon2,
			Icon3 = MyGUI::MessageBoxStyle::Icon3,
			Icon4 = MyGUI::MessageBoxStyle::Icon4,
			Icon5 = MyGUI::MessageBoxStyle::Icon5,
			Icon6 = MyGUI::MessageBoxStyle::Icon6,
			Icon7 = MyGUI::MessageBoxStyle::Icon7,
			Icon8 = MyGUI::MessageBoxStyle::Icon8
		};

		//#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<MyGUI::MessageBoxStyle>
		{
			typedef MessageBoxStyle Type;
			inline static const MessageBoxStyle& To(const MyGUI::MessageBoxStyle& _value) { return reinterpret_cast<const MessageBoxStyle&>(_value); }
			inline static MyGUI::MessageBoxStyle& From(MessageBoxStyle& _value) { return reinterpret_cast<MyGUI::MessageBoxStyle&>(_value); }
		};

	} // namespace Managed
} // namespace MyGUI
