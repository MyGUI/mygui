/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "Config.h"
#include "MouseButton.h"
#include "KeyCode.h"
#include "Marshaling.h"

namespace MyGUI
{
	namespace Managed
	{

		//#ifndef MMYGUI_USING_EXTERNAL_TYPE

		public value struct IBNotifyItemData
		{
			enum struct NotifyItem
			{
				MousePressed,
				MouseReleased,
				KeyPressed,
				KeyReleased
			};

			System::UInt32 index;
			NotifyItem notify;
			int x;
			int y;
			MouseButton id;
			KeyCode code;
			System::UInt32 key;
		};

		//#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<const MyGUI::IBNotifyItemData&>
		{
			typedef IBNotifyItemData Type;
			inline static const IBNotifyItemData& To(const MyGUI::IBNotifyItemData& _value)
			{
				return reinterpret_cast<const IBNotifyItemData&>(_value);
			}
			inline static MyGUI::IBNotifyItemData& From(IBNotifyItemData& _value)
			{
				return reinterpret_cast<MyGUI::IBNotifyItemData&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
