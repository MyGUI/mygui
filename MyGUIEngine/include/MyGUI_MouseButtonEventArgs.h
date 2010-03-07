/*!
	@file
	@author		Albert Semenov
	@date		03/2010
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
#ifndef __MYGUI_MOUSE_BUTTON_EVENT_ARGS_H__
#define __MYGUI_MOUSE_BUTTON_EVENT_ARGS_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_EventArgs.h"
#include "MyGUI_MouseButton.h"

namespace MyGUI
{

	class MYGUI_EXPORT MouseButtonEventArgs :
		public EventArgs
	{
		MYGUI_RTTI_DERIVED( MouseButtonEventArgs )

	public:
		MouseButtonEventArgs() : mX(0), mY(0) { }
		MouseButtonEventArgs(int _x, int _y, MouseButton _button) : mX(_x), mY(_y), mButton(_button) { }
		virtual ~MouseButtonEventArgs() { }

		int getX() { return mX; }
		int getY() { return mY; }
		MouseButton getButton() { return mButton; }

	private:
		int mX;
		int mY;
		MouseButton mButton;
	};

	typedef delegates::CMultiDelegate3<Widget*, EventInfo*, MouseButtonEventArgs*> MouseButtonEventHandler;

} // namespace MyGUI

#endif // __MYGUI_MOUSE_BUTTON_EVENT_ARGS_H__
