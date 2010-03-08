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

#include "MyGUI_Precompiled.h"
#include "MyGUI_InputElement.h"
#include "MyGUI_IEventCaller.h"

namespace MyGUI
{

	const char* gEventMouseEnterName = "EventMouseEnter";
	const char* gEventMouseLeaveName = "EventMouseLeave";

	const char* gEventMouseMoveName = "EventMouseMove";
	const char* gEventMouseDragName = "EventMouseDrag";
	const char* gEventMouseWheelName = "EventMouseWheel";
	const char* gEventMouseButtonDownName = "EventMouseButtonDown";
	const char* gEventMouseButtonUpName = "EventMouseButtonUp";
	const char* gEventMouseButtonClickName = "EventMouseButtonClick";
	const char* gEventMouseButtonDoubleClickName = "EventMouseButtonDoubleClick";
	const char* gEventGotKeyboardFocusName = "EventGotKeyboardFocus";
	const char* gEventLostKeyboardFocusName = "EventLostKeyboardFocus";
	const char* gEventKeyButtonDownName = "EventKeyButtonDown";
	const char* gEventKeyButtonUpName = "EventKeyButtonUp";

	const char* gEventPreviewMouseMoveName = "EventPreviewMouseMove";
	const char* gEventPreviewMouseDragName = "EventPreviewMouseDrag";
	const char* gEventPreviewMouseWheelName = "EventPreviewMouseWheel";
	const char* gEventPreviewMouseButtonDownName = "EventPreviewMouseButtonDown";
	const char* gEventPreviewMouseButtonUpName = "EventPreviewMouseButtonUp";
	const char* gEventPreviewMouseButtonClickName = "EventPreviewMouseButtonClick";
	const char* gEventPreviewMouseButtonDoubleClickName = "EventPreviewMouseButtonDoubleClick";
	const char* gEventPreviewGotKeyboardFocusName = "EventPreviewGotKeyboardFocus";
	const char* gEventPreviewLostKeyboardFocusName = "EventPreviewLostKeyboardFocus";
	const char* gEventPreviewKeyButtonDownName = "EventPreviewKeyButtonDown";
	const char* gEventPreviewKeyButtonUpName = "EventPreviewKeyButtonUp";

	InputElement::InputElement()
	{
	}

	InputElement::~InputElement()
	{
	}

	void InputElement::registerInputElement()
	{
		registerEvent(gEventMouseEnterName, false, false, new EventCaller<InputElement, EventArgs>(&InputElement::onEventMouseEnter));
		registerEvent(gEventMouseLeaveName, false, false, new EventCaller<InputElement, EventArgs>(&InputElement::onEventMouseLeave));

		registerEvent(gEventMouseMoveName, false, true, new EventCaller<InputElement, MouseMoveEventArgs>(&InputElement::onEventMouseMove));
		registerEvent(gEventMouseDragName, false, true, new EventCaller<InputElement, MouseMoveEventArgs>(&InputElement::onEventMouseDrag));
		registerEvent(gEventMouseWheelName, false, true, new EventCaller<InputElement, MouseWheelEventArgs>(&InputElement::onEventMouseWheel));
		registerEvent(gEventMouseButtonDownName, false, true, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventMouseButtonDown));
		registerEvent(gEventMouseButtonUpName, false, true, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventMouseButtonUp));
		registerEvent(gEventMouseButtonClickName, false, true, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventMouseButtonClick));
		registerEvent(gEventMouseButtonDoubleClickName, false, true, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventMouseButtonDoubleClick));
		registerEvent(gEventGotKeyboardFocusName, false, true, new EventCaller<InputElement, ChangeFocusEventArgs>(&InputElement::onEventGotKeyboardFocus));
		registerEvent(gEventLostKeyboardFocusName, false, true, new EventCaller<InputElement, ChangeFocusEventArgs>(&InputElement::onEventLostKeyboardFocus));
		registerEvent(gEventKeyButtonDownName, false, true, new EventCaller<InputElement, KeyButtonEventArgs>(&InputElement::onEventKeyButtonDown));
		registerEvent(gEventKeyButtonUpName, false, true, new EventCaller<InputElement, KeyButtonEventArgs>(&InputElement::onEventKeyButtonUp));

		registerEvent(gEventPreviewMouseMoveName, true, false, new EventCaller<InputElement, MouseMoveEventArgs>(&InputElement::onEventPreviewMouseMove));
		registerEvent(gEventPreviewMouseDragName, true, false, new EventCaller<InputElement, MouseMoveEventArgs>(&InputElement::onEventPreviewMouseDrag));
		registerEvent(gEventPreviewMouseWheelName, true, false, new EventCaller<InputElement, MouseWheelEventArgs>(&InputElement::onEventPreviewMouseWheel));
		registerEvent(gEventPreviewMouseButtonDownName, true, false, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventPreviewMouseButtonDown));
		registerEvent(gEventPreviewMouseButtonUpName, true, false, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventPreviewMouseButtonUp));
		registerEvent(gEventPreviewMouseButtonClickName, true, false, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventPreviewMouseButtonClick));
		registerEvent(gEventPreviewMouseButtonDoubleClickName, true, false, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventPreviewMouseButtonDoubleClick));
		registerEvent(gEventPreviewGotKeyboardFocusName, true, false, new EventCaller<InputElement, ChangeFocusEventArgs>(&InputElement::onEventPreviewGotKeyboardFocus));
		registerEvent(gEventPreviewLostKeyboardFocusName, true, false, new EventCaller<InputElement, ChangeFocusEventArgs>(&InputElement::onEventPreviewLostKeyboardFocus));
		registerEvent(gEventPreviewKeyButtonDownName, true, false, new EventCaller<InputElement, KeyButtonEventArgs>(&InputElement::onEventPreviewKeyButtonDown));
		registerEvent(gEventPreviewKeyButtonUpName, true, false, new EventCaller<InputElement, KeyButtonEventArgs>(&InputElement::onEventPreviewKeyButtonUp));
	}

	void InputElement::unregisterInputElement()
	{
		unregisterEvent(gEventMouseEnterName);
		unregisterEvent(gEventMouseLeaveName);

		unregisterEvent(gEventMouseMoveName);
		unregisterEvent(gEventMouseDragName);
		unregisterEvent(gEventMouseWheelName);
		unregisterEvent(gEventMouseButtonDownName);
		unregisterEvent(gEventMouseButtonUpName);
		unregisterEvent(gEventMouseButtonClickName);
		unregisterEvent(gEventMouseButtonDoubleClickName);
		unregisterEvent(gEventGotKeyboardFocusName);
		unregisterEvent(gEventLostKeyboardFocusName);
		unregisterEvent(gEventKeyButtonDownName);
		unregisterEvent(gEventKeyButtonUpName);

		unregisterEvent(gEventPreviewMouseMoveName);
		unregisterEvent(gEventPreviewMouseDragName);
		unregisterEvent(gEventPreviewMouseWheelName);
		unregisterEvent(gEventPreviewMouseButtonDownName);
		unregisterEvent(gEventPreviewMouseButtonUpName);
		unregisterEvent(gEventPreviewMouseButtonClickName);
		unregisterEvent(gEventPreviewMouseButtonDoubleClickName);
		unregisterEvent(gEventPreviewGotKeyboardFocusName);
		unregisterEvent(gEventPreviewLostKeyboardFocusName);
		unregisterEvent(gEventPreviewKeyButtonDownName);
		unregisterEvent(gEventPreviewKeyButtonUpName);
	}

	void InputElement::raiseEventMouseEnter(EventArgs* _args)
	{
		raiseEvent(gEventMouseEnterName, _args);
	}

	void InputElement::raiseEventMouseLeave(EventArgs* _args)
	{
		raiseEvent(gEventMouseLeaveName, _args);
	}


	void InputElement::raiseEventMouseMove(MouseMoveEventArgs* _args)
	{
		raiseEvent(gEventPreviewMouseMoveName, _args);
		raiseEvent(gEventMouseMoveName, _args);
	}

	void InputElement::raiseEventMouseDrag(MouseMoveEventArgs* _args)
	{
		raiseEvent(gEventPreviewMouseDragName, _args);
		raiseEvent(gEventMouseDragName, _args);
	}

	void InputElement::raiseEventMouseWheel(MouseWheelEventArgs* _args)
	{
		raiseEvent(gEventPreviewMouseWheelName, _args);
		raiseEvent(gEventMouseWheelName, _args);
	}

	void InputElement::raiseEventMouseButtonDown(MouseButtonEventArgs* _args)
	{
		raiseEvent(gEventPreviewMouseButtonDownName, _args);
		raiseEvent(gEventMouseButtonDownName, _args);
	}

	void InputElement::raiseEventMouseButtonUp(MouseButtonEventArgs* _args)
	{
		raiseEvent(gEventPreviewMouseButtonUpName, _args);
		raiseEvent(gEventMouseButtonUpName, _args);
	}

	void InputElement::raiseEventMouseButtonClick(MouseButtonEventArgs* _args)
	{
		raiseEvent(gEventPreviewMouseButtonClickName, _args);
		raiseEvent(gEventMouseButtonClickName, _args);
	}

	void InputElement::raiseEventMouseButtonDoubleClick(MouseButtonEventArgs* _args)
	{
		raiseEvent(gEventPreviewMouseButtonDoubleClickName, _args);
		raiseEvent(gEventMouseButtonDoubleClickName, _args);
	}

	void InputElement::raiseEventGotKeyboardFocus(ChangeFocusEventArgs* _args)
	{
		raiseEvent(gEventPreviewGotKeyboardFocusName, _args);
		raiseEvent(gEventGotKeyboardFocusName, _args);
	}

	void InputElement::raiseEventLostKeyboardFocus(ChangeFocusEventArgs* _args)
	{
		raiseEvent(gEventPreviewLostKeyboardFocusName, _args);
		raiseEvent(gEventLostKeyboardFocusName, _args);
	}

	void InputElement::raiseEventKeyButtonDown(KeyButtonEventArgs* _args)
	{
		raiseEvent(gEventPreviewKeyButtonDownName, _args);
		raiseEvent(gEventKeyButtonDownName, _args);
	}

	void InputElement::raiseEventKeyButtonUp(KeyButtonEventArgs* _args)
	{
		raiseEvent(gEventPreviewKeyButtonUpName, _args);
		raiseEvent(gEventKeyButtonUpName, _args);
	}


	void InputElement::onEventMouseEnter(Widget* _sender, EventInfo* _info, EventArgs* _args)
	{
		EventMouseEnter(_sender, _info, _args);
	}

	void InputElement::onEventMouseLeave(Widget* _sender, EventInfo* _info, EventArgs* _args)
	{
		EventMouseLeave(_sender, _info, _args);
	}


	void InputElement::onEventMouseMove(Widget* _sender, EventInfo* _info, MouseMoveEventArgs* _args)
	{
		EventMouseMove(_sender, _info, _args);
	}

	void InputElement::onEventMouseDrag(Widget* _sender, EventInfo* _info, MouseMoveEventArgs* _args)
	{
		EventMouseDrag(_sender, _info, _args);
	}

	void InputElement::onEventMouseWheel(Widget* _sender, EventInfo* _info, MouseWheelEventArgs* _args)
	{
		EventMouseWheel(_sender, _info, _args);
	}

	void InputElement::onEventMouseButtonDown(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args)
	{
		EventMouseButtonDown(_sender, _info, _args);
	}

	void InputElement::onEventMouseButtonUp(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args)
	{
		EventMouseButtonUp(_sender, _info, _args);
	}

	void InputElement::onEventMouseButtonClick(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args)
	{
		EventMouseButtonClick(_sender, _info, _args);
	}

	void InputElement::onEventMouseButtonDoubleClick(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args)
	{
		EventMouseButtonDoubleClick(_sender, _info, _args);
	}

	void InputElement::onEventGotKeyboardFocus(Widget* _sender, EventInfo* _info, ChangeFocusEventArgs* _args)
	{
		EventGotKeyboardFocus(_sender, _info, _args);
	}

	void InputElement::onEventLostKeyboardFocus(Widget* _sender, EventInfo* _info, ChangeFocusEventArgs* _args)
	{
		EventLostKeyboardFocus(_sender, _info, _args);
	}

	void InputElement::onEventKeyButtonDown(Widget* _sender, EventInfo* _info, KeyButtonEventArgs* _args)
	{
		EventKeyButtonDown(_sender, _info, _args);
	}

	void InputElement::onEventKeyButtonUp(Widget* _sender, EventInfo* _info, KeyButtonEventArgs* _args)
	{
		EventKeyButtonUp(_sender, _info, _args);
	}


	void InputElement::onEventPreviewMouseMove(Widget* _sender, EventInfo* _info, MouseMoveEventArgs* _args)
	{
		EventPreviewMouseMove(_sender, _info, _args);
	}

	void InputElement::onEventPreviewMouseDrag(Widget* _sender, EventInfo* _info, MouseMoveEventArgs* _args)
	{
		EventPreviewMouseDrag(_sender, _info, _args);
	}

	void InputElement::onEventPreviewMouseWheel(Widget* _sender, EventInfo* _info, MouseWheelEventArgs* _args)
	{
		EventPreviewMouseWheel(_sender, _info, _args);
	}

	void InputElement::onEventPreviewMouseButtonDown(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args)
	{
		EventPreviewMouseButtonDown(_sender, _info, _args);
	}

	void InputElement::onEventPreviewMouseButtonUp(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args)
	{
		EventPreviewMouseButtonUp(_sender, _info, _args);
	}

	void InputElement::onEventPreviewMouseButtonClick(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args)
	{
		EventPreviewMouseButtonClick(_sender, _info, _args);
	}

	void InputElement::onEventPreviewMouseButtonDoubleClick(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args)
	{
		EventPreviewMouseButtonDoubleClick(_sender, _info, _args);
	}

	void InputElement::onEventPreviewGotKeyboardFocus(Widget* _sender, EventInfo* _info, ChangeFocusEventArgs* _args)
	{
		EventPreviewGotKeyboardFocus(_sender, _info, _args);
	}

	void InputElement::onEventPreviewLostKeyboardFocus(Widget* _sender, EventInfo* _info, ChangeFocusEventArgs* _args)
	{
		EventPreviewLostKeyboardFocus(_sender, _info, _args);
	}

	void InputElement::onEventPreviewKeyButtonDown(Widget* _sender, EventInfo* _info, KeyButtonEventArgs* _args)
	{
		EventPreviewKeyButtonDown(_sender, _info, _args);
	}

	void InputElement::onEventPreviewKeyButtonUp(Widget* _sender, EventInfo* _info, KeyButtonEventArgs* _args)
	{
		EventPreviewKeyButtonUp(_sender, _info, _args);
	}

} // namespace MyGUI
