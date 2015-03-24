/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_MOUSE_BUTTON_H_
#define MYGUI_MOUSE_BUTTON_H_

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	struct MYGUI_EXPORT MouseButton
	{
		enum Enum
		{
			None = -1,

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
			MAX
		};

		MouseButton(Enum _value = None) :
			mValue(_value)
		{
		}

		friend bool operator == (MouseButton const& a, MouseButton const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator != (MouseButton const& a, MouseButton const& b)
		{
			return a.mValue != b.mValue;
		}

		int getValue() const
		{
			return mValue;
		}

	private:
		Enum mValue;
	};

} // namespace MyGUI

#endif // MYGUI_MOUSE_BUTTON_H_
