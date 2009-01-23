/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>

namespace MMyGUI
{

	public value struct ItemInfo
	{
		size_t index;
		bool update;
		bool select;
		bool active;
		bool drag;
		bool drag_accept;
		bool drag_refuse;
	};

} // namespace MMyGUI
