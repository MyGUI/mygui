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

	InputElement::InputElement() :
		mRootMouseActive(false),
		mRootKeyActive(false)
	{
	}

	InputElement::~InputElement()
	{
	}

	void InputElement::registerInputElement()
	{
		registerEvent(gEventMouseEnterName, false, false, new EventCaller<InputElement, EventArgs>(&InputElement::onEventMouseEnter, &InputElement::EventMouseEnter));
		registerEvent(gEventMouseLeaveName, false, false, new EventCaller<InputElement, EventArgs>(&InputElement::onEventMouseLeave, &InputElement::EventMouseLeave));

		registerEvent(gEventMouseMoveName, false, true, new EventCaller<InputElement, MouseMoveEventArgs>(&InputElement::onEventMouseMove, &InputElement::EventMouseMove));
		registerEvent(gEventMouseDragName, false, true, new EventCaller<InputElement, MouseMoveEventArgs>(&InputElement::onEventMouseDrag, &InputElement::EventMouseDrag));
		registerEvent(gEventMouseWheelName, false, true, new EventCaller<InputElement, MouseWheelEventArgs>(&InputElement::onEventMouseWheel, &InputElement::EventMouseWheel));
		registerEvent(gEventMouseButtonDownName, false, true, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventMouseButtonDown, &InputElement::EventMouseButtonDown));
		registerEvent(gEventMouseButtonUpName, false, true, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventMouseButtonUp, &InputElement::EventMouseButtonUp));
		registerEvent(gEventMouseButtonClickName, false, true, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventMouseButtonClick, &InputElement::EventMouseButtonClick));
		registerEvent(gEventMouseButtonDoubleClickName, false, true, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventMouseButtonDoubleClick, &InputElement::EventMouseButtonDoubleClick));
		registerEvent(gEventGotKeyboardFocusName, false, true, new EventCaller<InputElement, ChangeFocusEventArgs>(&InputElement::onEventGotKeyboardFocus, &InputElement::EventGotKeyboardFocus));
		registerEvent(gEventLostKeyboardFocusName, false, true, new EventCaller<InputElement, ChangeFocusEventArgs>(&InputElement::onEventLostKeyboardFocus, &InputElement::EventLostKeyboardFocus));
		registerEvent(gEventKeyButtonDownName, false, true, new EventCaller<InputElement, KeyButtonEventArgs>(&InputElement::onEventKeyButtonDown, &InputElement::EventKeyButtonDown));
		registerEvent(gEventKeyButtonUpName, false, true, new EventCaller<InputElement, KeyButtonEventArgs>(&InputElement::onEventKeyButtonUp, &InputElement::EventKeyButtonUp));

		registerEvent(gEventPreviewMouseMoveName, true, false, new EventCaller<InputElement, MouseMoveEventArgs>(&InputElement::onEventPreviewMouseMove, &InputElement::EventPreviewMouseMove));
		registerEvent(gEventPreviewMouseDragName, true, false, new EventCaller<InputElement, MouseMoveEventArgs>(&InputElement::onEventPreviewMouseDrag, &InputElement::EventPreviewMouseDrag));
		registerEvent(gEventPreviewMouseWheelName, true, false, new EventCaller<InputElement, MouseWheelEventArgs>(&InputElement::onEventPreviewMouseWheel, &InputElement::EventPreviewMouseWheel));
		registerEvent(gEventPreviewMouseButtonDownName, true, false, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventPreviewMouseButtonDown, &InputElement::EventPreviewMouseButtonDown));
		registerEvent(gEventPreviewMouseButtonUpName, true, false, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventPreviewMouseButtonUp, &InputElement::EventPreviewMouseButtonUp));
		registerEvent(gEventPreviewMouseButtonClickName, true, false, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventPreviewMouseButtonClick, &InputElement::EventPreviewMouseButtonClick));
		registerEvent(gEventPreviewMouseButtonDoubleClickName, true, false, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventPreviewMouseButtonDoubleClick, &InputElement::EventPreviewMouseButtonDoubleClick));
		registerEvent(gEventPreviewGotKeyboardFocusName, true, false, new EventCaller<InputElement, ChangeFocusEventArgs>(&InputElement::onEventPreviewGotKeyboardFocus, &InputElement::EventPreviewGotKeyboardFocus));
		registerEvent(gEventPreviewLostKeyboardFocusName, true, false, new EventCaller<InputElement, ChangeFocusEventArgs>(&InputElement::onEventPreviewLostKeyboardFocus, &InputElement::EventPreviewLostKeyboardFocus));
		registerEvent(gEventPreviewKeyButtonDownName, true, false, new EventCaller<InputElement, KeyButtonEventArgs>(&InputElement::onEventPreviewKeyButtonDown, &InputElement::EventPreviewKeyButtonDown));
		registerEvent(gEventPreviewKeyButtonUpName, true, false, new EventCaller<InputElement, KeyButtonEventArgs>(&InputElement::onEventPreviewKeyButtonUp, &InputElement::EventPreviewKeyButtonUp));
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

} // namespace MyGUI
