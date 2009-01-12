/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_MOUSE_BUTTON_H__
#define __MMYGUI_MOUSE_BUTTON_H__

#include <MyGUI.h>

namespace MMyGUI
{

	public enum class MouseButton
	{
		Left = 0,
		Right,
		Middle,

		Button0 = 0,
		Button1,
		Button2,
		Button3,
		Button4,
		Button5,
		Button6,
		Button7,

		None = -1
	};

} // namespace MMyGUI

#endif // __MMYGUI_MOUSE_BUTTON_H__
