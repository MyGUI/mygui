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

	ref class Widget;

	public value struct ItemDropInfo
	{
		Widget^ sender;
		System::UInt32 sender_index;
		Widget^ reseiver;
		System::UInt32 reseiver_index;
	};

} // namespace MMyGUI
