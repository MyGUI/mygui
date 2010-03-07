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
#ifndef __MYGUI_MOUSE_MOVE_EVENT_ARGS_H__
#define __MYGUI_MOUSE_MOVE_EVENT_ARGS_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_EventArgs.h"

namespace MyGUI
{

	class MYGUI_EXPORT MouseMoveEventArgs :
		public EventArgs
	{
		MYGUI_RTTI_DERIVED( MouseMoveEventArgs )

	public:
		MouseMoveEventArgs() : mX(0), mY(0) { }
		MouseMoveEventArgs(int _x, int _y) : mX(_x), mY(_y) { }
		virtual ~MouseMoveEventArgs() { }

		int getX() { return mX; }
		int getY() { return mY; }

	private:
		int mX;
		int mY;
	};

	typedef delegates::CMultiDelegate3<Widget*, EventInfo*, MouseMoveEventArgs*> MouseMoveEventHandler;

} // namespace MyGUI

#endif // __MYGUI_MOUSE_MOVE_EVENT_ARGS_H__
