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

		public enum struct ResizingPolicy
		{
			Auto = MyGUI::ResizingPolicy::Auto,
			Fixed = MyGUI::ResizingPolicy::Fixed,
			Fill = MyGUI::ResizingPolicy::Fill
		};

		//#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<MyGUI::ResizingPolicy>
		{
			typedef ResizingPolicy Type;
			inline static const ResizingPolicy& To(const MyGUI::ResizingPolicy& _value)
			{
				return reinterpret_cast<const ResizingPolicy&>(_value);
			}
			inline static MyGUI::ResizingPolicy& From(ResizingPolicy& _value)
			{
				return reinterpret_cast<MyGUI::ResizingPolicy&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
