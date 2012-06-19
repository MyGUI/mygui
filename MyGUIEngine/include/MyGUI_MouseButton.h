/*!
	@file
	@author		Albert Semenov
	@date		03/2008
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_MOUSE_BUTTON_H__
#define __MYGUI_MOUSE_BUTTON_H__

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

#endif // __MYGUI_MOUSE_BUTTON_H__
