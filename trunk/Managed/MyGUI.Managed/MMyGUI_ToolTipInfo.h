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
#include "MMyGUI_IntPoint.h"

namespace MyGUI
{
	namespace Managed
	{

		//#ifndef MMYGUI_USING_EXTERNAL_TYPE

		public value struct ToolTipInfo
		{
			bool visible;
			System::UInt32 index;
			IntPoint point;
		};

		//#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<const MyGUI::ToolTipInfo&>
		{
			typedef ToolTipInfo Type;
			inline static const ToolTipInfo& To(const MyGUI::ToolTipInfo& _value) { return reinterpret_cast<const ToolTipInfo&>(_value); }
			inline static MyGUI::ToolTipInfo& From(ToolTipInfo& _value) { return reinterpret_cast<MyGUI::ToolTipInfo&>(_value); }
		};

	} // namespace Managed
} // namespace MyGUI
