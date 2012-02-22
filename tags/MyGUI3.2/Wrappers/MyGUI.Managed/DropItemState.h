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

		public enum struct DDItemState
		{
			None = MyGUI::DDItemState::None,
			Start = MyGUI::DDItemState::Start,
			End = MyGUI::DDItemState::End,
			Miss = MyGUI::DDItemState::Miss,
			Accept = MyGUI::DDItemState::Accept,
			Refuse = MyGUI::DDItemState::Refuse
		};

		//#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<MyGUI::DDItemState>
		{
			typedef DDItemState Type;
			inline static const DDItemState& To(const MyGUI::DDItemState& _value)
			{
				return reinterpret_cast<const DDItemState&>(_value);
			}
			inline static MyGUI::DDItemState& From(DDItemState& _value)
			{
				return reinterpret_cast<MyGUI::DDItemState&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
