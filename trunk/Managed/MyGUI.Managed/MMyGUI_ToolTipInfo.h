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

namespace MMyGUI
{

#ifndef MMYGUI_USING_INTERFACE

	public value struct ToolTipInfo
	{
		bool visible;
		System::UInt32 index;
		int x;
		int y;
	};

#endif // MMYGUI_USING_INTERFACE

	template <> struct Convert<const MyGUI::ToolTipInfo&>
	{
		typedef ToolTipInfo Type;
		inline static const ToolTipInfo& To(const MyGUI::ToolTipInfo& _value) { return reinterpret_cast<const ToolTipInfo&>(_value); }
		inline static MyGUI::ToolTipInfo& From(ToolTipInfo& _value) { return reinterpret_cast<MyGUI::ToolTipInfo&>(_value); }
	};

} // namespace MMyGUI
