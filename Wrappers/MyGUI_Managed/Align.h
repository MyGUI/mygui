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

		public enum struct Align
		{
			HCenter = MyGUI::Align::HCenter,
			VCenter = MyGUI::Align::VCenter,
			Center = MyGUI::Align::Center,

			Left = MyGUI::Align::Left,
			Right = MyGUI::Align::Right,
			HStretch = MyGUI::Align::HStretch,

			Top = MyGUI::Align::Top,
			Bottom = MyGUI::Align::Bottom,
			VStretch = MyGUI::Align::VStretch,

			Stretch = MyGUI::Align::Stretch,
			Default = MyGUI::Align::Default
		};

		//#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<MyGUI::Align>
		{
			typedef Align Type;
			inline static const Align& To(const MyGUI::Align& _value)
			{
				return reinterpret_cast<const Align&>(_value);
			}
			inline static MyGUI::Align& From(Align& _value)
			{
				return reinterpret_cast<MyGUI::Align&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
