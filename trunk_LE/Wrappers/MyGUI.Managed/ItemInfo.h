/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "Marshaling.h"

namespace MyGUI
{
	namespace Managed
	{

		//#ifndef MMYGUI_USING_EXTERNAL_TYPE

		public value struct IBDrawItemInfo
		{
			System::UInt32 index;
			bool update;
			bool select;
			bool active;
			bool drag;
			bool drop_accept;
			bool drop_refuse;
		};

		//#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<const MyGUI::IBDrawItemInfo&>
		{
			typedef IBDrawItemInfo Type;
			inline static const IBDrawItemInfo& To(const MyGUI::IBDrawItemInfo& _value)
			{
				return reinterpret_cast<const IBDrawItemInfo&>(_value);
			}
			inline static MyGUI::IBDrawItemInfo& From(IBDrawItemInfo& _value)
			{
				return reinterpret_cast<MyGUI::IBDrawItemInfo&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
