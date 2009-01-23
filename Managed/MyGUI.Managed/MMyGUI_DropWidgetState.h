/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Macros.h"

namespace MMyGUI
{

	public value struct DropWidgetState
	{
		//static operator const DropWidgetState& ( const MyGUI::DropWidgetState& obj) { return reinterpret_cast<const DropWidgetState&>(obj); }

		size_t index;
		bool update;
		bool accept;
		bool refuse;
	};

} // namespace MMyGUI
