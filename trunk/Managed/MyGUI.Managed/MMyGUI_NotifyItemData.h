/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Macros.h"
#include "MMyGUI_MouseButton.h"
#include "MMyGUI_KeyCode.h"
#include "MMyGUI_KeyCode.h"

namespace MMyGUI
{

	ref class Widget;

	public value struct NotifyItemData
	{
		enum struct NotifyItem
		{
			MousePressed,
			MouseReleased,
			KeyPressed,
			KeyReleased
		};

		size_t index;
		NotifyItem notify;
		int x;
		int y;
		MouseButton id;
		KeyCode code;
		System::UInt32 key;
	};

} // namespace MMyGUI
