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

	public value struct DropWidgetState
	{
		System::UInt32 index;
		bool update;
		bool accept;
		bool refuse;
	};

	template <> struct Convert<const MyGUI::DropWidgetState&>
	{
		typedef DropWidgetState Type;
		inline static const DropWidgetState& To(const MyGUI::DropWidgetState& _value) { return reinterpret_cast<const DropWidgetState&>(_value); }
		inline static MyGUI::DropWidgetState& From(DropWidgetState& _value) { return reinterpret_cast<MyGUI::DropWidgetState&>(_value); }
	};

} // namespace MMyGUI
