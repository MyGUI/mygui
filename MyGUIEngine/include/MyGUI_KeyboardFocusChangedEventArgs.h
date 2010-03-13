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
#ifndef __MYGUI_KEYBOARD_FOCUS_CHANGED_EVENT_ARGS_H__
#define __MYGUI_KEYBOARD_FOCUS_CHANGED_EVENT_ARGS_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_FocusChangedEventArgs.h"

namespace MyGUI
{

	class MYGUI_EXPORT KeyboardFocusChangedEventArgs :
		public FocusChangedEventArgs
	{
		MYGUI_RTTI_DERIVED( KeyboardFocusChangedEventArgs )

	public:
		KeyboardFocusChangedEventArgs() : mOldFocus(nullptr), mNewFocus(nullptr) { }
		KeyboardFocusChangedEventArgs(Widget* _old, Widget* _new, bool _focus) : FocusChangedEventArgs(_focus), mOldFocus(_old), mNewFocus(_new) { }
		virtual ~KeyboardFocusChangedEventArgs() { }

		Widget* getOldFocus() { return mOldFocus; }
		Widget* getNewFocus() { return mNewFocus; }

	private:
		Widget* mOldFocus;
		Widget* mNewFocus;
	};

	typedef delegates::CMultiDelegate3<Widget*, EventInfo*, KeyboardFocusChangedEventArgs*> KeyboardFocusChangedEventArgsHandler;

} // namespace MyGUI

#endif // __MYGUI_KEYBOARD_FOCUS_CHANGED_EVENT_ARGS_H__
