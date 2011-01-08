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

		public enum struct FlowDirection
		{
			LeftToRight = MyGUI::FlowDirection::LeftToRight,
			RightToLeft = MyGUI::FlowDirection::RightToLeft,
			TopToBottom = MyGUI::FlowDirection::TopToBottom,
			BottomToTop = MyGUI::FlowDirection::BottomToTop
		};

		//#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<MyGUI::FlowDirection>
		{
			typedef FlowDirection Type;
			inline static const FlowDirection& To(const MyGUI::FlowDirection& _value)
			{
				return reinterpret_cast<const FlowDirection&>(_value);
			}
			inline static MyGUI::FlowDirection& From(FlowDirection& _value)
			{
				return reinterpret_cast<MyGUI::FlowDirection&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
