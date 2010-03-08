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
#ifndef __MYGUI_INPUT_ELEMENT_H__
#define __MYGUI_INPUT_ELEMENT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_UIElement.h"
#include "MyGUI_ChangeFocusEventArgs.h"
#include "MyGUI_KeyButtonEventArgs.h"
#include "MyGUI_MouseButtonEventArgs.h"
#include "MyGUI_MouseMoveEventArgs.h"
#include "MyGUI_MouseWheelEventArgs.h"

namespace MyGUI
{

	class MYGUI_EXPORT InputElement :
		public UIElement
	{
		MYGUI_RTTI_DERIVED( InputElement )

	public:
		InputElement();
		virtual ~InputElement();

		RoutedEventHandler EventMouseEnter;
		RoutedEventHandler EventMouseLeave;

		MouseMoveEventHandler EventMouseMove;
		MouseMoveEventHandler EventMouseDrag;
		MouseWheelEventHandler  EventMouseWheel;
		MouseButtonEventHandler EventMouseButtonDown;
		MouseButtonEventHandler EventMouseButtonUp;
		MouseButtonEventHandler EventMouseButtonClick;
		MouseButtonEventHandler EventMouseButtonDoubleClick;
		ChangeFocusEventHandler EventGotKeyboardFocus;
		ChangeFocusEventHandler EventLostKeyboardFocus;
		KeyButtonEventHandler EventKeyButtonDown;
		KeyButtonEventHandler EventKeyButtonUp;

		MouseMoveEventHandler EventPreviewMouseMove;
		MouseMoveEventHandler EventPreviewMouseDrag;
		MouseWheelEventHandler  EventPreviewMouseWheel;
		MouseButtonEventHandler EventPreviewMouseButtonDown;
		MouseButtonEventHandler EventPreviewMouseButtonUp;
		MouseButtonEventHandler EventPreviewMouseButtonClick;
		MouseButtonEventHandler EventPreviewMouseButtonDoubleClick;
		ChangeFocusEventHandler EventPreviewGotKeyboardFocus;
		ChangeFocusEventHandler EventPreviewLostKeyboardFocus;
		KeyButtonEventHandler EventPreviewKeyButtonDown;
		KeyButtonEventHandler EventPreviewKeyButtonUp;

	/*internal:*/
		void raiseEventMouseEnter(EventArgs* _args);
		void raiseEventMouseLeave(EventArgs* _args);

		void raiseEventMouseMove(MouseMoveEventArgs* _args);
		void raiseEventMouseDrag(MouseMoveEventArgs* _args);
		void raiseEventMouseWheel(MouseWheelEventArgs* _args);
		void raiseEventMouseButtonDown(MouseButtonEventArgs* _args);
		void raiseEventMouseButtonUp(MouseButtonEventArgs* _args);
		void raiseEventMouseButtonClick(MouseButtonEventArgs* _args);
		void raiseEventMouseButtonDoubleClick(MouseButtonEventArgs* _args);
		void raiseEventGotKeyboardFocus(ChangeFocusEventArgs* _args);
		void raiseEventLostKeyboardFocus(ChangeFocusEventArgs* _args);
		void raiseEventKeyButtonDown(KeyButtonEventArgs* _args);
		void raiseEventKeyButtonUp(KeyButtonEventArgs* _args);

	protected:
		static void registerInputElement();
		static void unregisterInputElement();

		virtual void onEventMouseEnter(Widget* _sender, EventInfo* _info, EventArgs* _args);
		virtual void onEventMouseLeave(Widget* _sender, EventInfo* _info, EventArgs* _args);

		virtual void onEventMouseMove(Widget* _sender, EventInfo* _info, MouseMoveEventArgs* _args);
		virtual void onEventMouseDrag(Widget* _sender, EventInfo* _info, MouseMoveEventArgs* _args);
		virtual void onEventMouseWheel(Widget* _sender, EventInfo* _info, MouseWheelEventArgs* _args);
		virtual void onEventMouseButtonDown(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args);
		virtual void onEventMouseButtonUp(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args);
		virtual void onEventMouseButtonClick(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args);
		virtual void onEventMouseButtonDoubleClick(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args);
		virtual void onEventGotKeyboardFocus(Widget* _sender, EventInfo* _info, ChangeFocusEventArgs* _args);
		virtual void onEventLostKeyboardFocus(Widget* _sender, EventInfo* _info, ChangeFocusEventArgs* _args);
		virtual void onEventKeyButtonDown(Widget* _sender, EventInfo* _info, KeyButtonEventArgs* _args);
		virtual void onEventKeyButtonUp(Widget* _sender, EventInfo* _info, KeyButtonEventArgs* _args);

		virtual void onEventPreviewMouseMove(Widget* _sender, EventInfo* _info, MouseMoveEventArgs* _args);
		virtual void onEventPreviewMouseDrag(Widget* _sender, EventInfo* _info, MouseMoveEventArgs* _args);
		virtual void onEventPreviewMouseWheel(Widget* _sender, EventInfo* _info, MouseWheelEventArgs* _args);
		virtual void onEventPreviewMouseButtonDown(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args);
		virtual void onEventPreviewMouseButtonUp(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args);
		virtual void onEventPreviewMouseButtonClick(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args);
		virtual void onEventPreviewMouseButtonDoubleClick(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args);
		virtual void onEventPreviewGotKeyboardFocus(Widget* _sender, EventInfo* _info, ChangeFocusEventArgs* _args);
		virtual void onEventPreviewLostKeyboardFocus(Widget* _sender, EventInfo* _info, ChangeFocusEventArgs* _args);
		virtual void onEventPreviewKeyButtonDown(Widget* _sender, EventInfo* _info, KeyButtonEventArgs* _args);
		virtual void onEventPreviewKeyButtonUp(Widget* _sender, EventInfo* _info, KeyButtonEventArgs* _args);
	};

} // namespace MyGUI

#endif // __MYGUI_INPUT_ELEMENT_H__
