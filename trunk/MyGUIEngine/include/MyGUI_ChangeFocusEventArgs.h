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
#ifndef __MYGUI_CHANGE_FOCUS_EVENT_ARGS_H__
#define __MYGUI_CHANGE_FOCUS_EVENT_ARGS_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_EventArgs.h"

namespace MyGUI
{

	class MYGUI_EXPORT ChangeFocusEventArgs :
		public EventArgs
	{
		MYGUI_RTTI_DERIVED( ChangeFocusEventArgs )

	public:
		ChangeFocusEventArgs() : mOld(nullptr), mNew(nullptr) { }
		ChangeFocusEventArgs(Widget* _old, Widget* _new) : mOld(_old), mNew(_new) { }
		virtual ~ChangeFocusEventArgs() { }

		Widget* getOld() { return mOld; }
		Widget* getNew() { return mNew; }

	private:
		Widget* mOld;
		Widget* mNew;
	};

	typedef delegates::CMultiDelegate3<Widget*, EventInfo*, ChangeFocusEventArgs*> ChangeFocusEventHandler;

} // namespace MyGUI

#endif // __MYGUI_CHANGE_FOCUS_EVENT_ARGS_H__
