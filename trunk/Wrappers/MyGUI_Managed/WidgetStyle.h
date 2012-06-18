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

		public enum struct WidgetStyle
		{
			Overlapped = MyGUI::WidgetStyle::Overlapped,
			Child = MyGUI::WidgetStyle::Child,
			Popup = MyGUI::WidgetStyle::Popup
		};

		//#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<MyGUI::WidgetStyle>
		{
			typedef WidgetStyle Type;
			inline static const WidgetStyle& To(const MyGUI::WidgetStyle& _value)
			{
				return reinterpret_cast<const WidgetStyle&>(_value);
			}
			inline static MyGUI::WidgetStyle& From(WidgetStyle& _value)
			{
				return reinterpret_cast<MyGUI::WidgetStyle&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
