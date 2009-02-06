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

	public value struct ToolTipInfo
	{
		bool visible;
		size_t index;
		int x;
		int y;
	};

} // namespace MMyGUI
