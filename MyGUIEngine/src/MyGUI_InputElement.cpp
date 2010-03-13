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
#include "MyGUI_InputManager.h"

namespace MyGUI
{

	const char* gEventMouseFocusChangedName = "EventMouseFocusChanged";
	const char* gEventMouseRootFocusChangedName = "EventMouseRootFocusChanged";
	const char* gEventKeyboardRootFocusChangedName = "EventKeyboardRootFocusChanged";

	const char* gEventPreviewMouseMoveName = "EventPreviewMouseMove";
	const char* gEventPreviewMouseDragName = "EventPreviewMouseDrag";
	const char* gEventPreviewMouseWheelName = "EventPreviewMouseWheel";
	const char* gEventPreviewMouseButtonDownName = "EventPreviewMouseButtonDown";
	const char* gEventPreviewMouseButtonUpName = "EventPreviewMouseButtonUp";
	const char* gEventPreviewMouseButtonClickName = "EventPreviewMouseButtonClick";
	const char* gEventPreviewMouseButtonDoubleClickName = "EventPreviewMouseButtonDoubleClick";
	const char* gEventPreviewKeyboardFocusChangedName = "EventPreviewKeyboardFocusChanged";
	const char* gEventPreviewKeyButtonDownName = "EventPreviewKeyButtonDown";
	const char* gEventPreviewKeyButtonUpName = "EventPreviewKeyButtonUp";

	const char* gEventMouseMoveName = "EventMouseMove";
	const char* gEventMouseDragName = "EventMouseDrag";
	const char* gEventMouseWheelName = "EventMouseWheel";
	const char* gEventMouseButtonDownName = "EventMouseButtonDown";
	const char* gEventMouseButtonUpName = "EventMouseButtonUp";
	const char* gEventMouseButtonClickName = "EventMouseButtonClick";
	const char* gEventMouseButtonDoubleClickName = "EventMouseButtonDoubleClick";
	const char* gEventKeyboardFocusChangedName = "EventKeyboardFocusChanged";
	const char* gEventKeyButtonDownName = "EventKeyButtonDown";
	const char* gEventKeyButtonUpName = "EventKeyButtonUp";

	InputElement::InputElement() :
		mMouseRootFocus(false),
		mKeyboardRootFocus(false),
		mNeedKeyFocus(false),
		mNeedMouseFocus(true),
		mEnabled(true),
		mInheritsEnabled(true),
		mVisible(true),
		mInheritsVisible(true)
	{
	}

	InputElement::~InputElement()
	{
	}

	void InputElement::registerInputElement()
	{
		registerEvent(gEventMouseFocusChangedName, EventType::Direct, new EventCaller<InputElement, FocusChangedEventArgs>(&InputElement::onEventMouseFocusChanged, &InputElement::EventMouseFocusChanged));
		registerEvent(gEventMouseRootFocusChangedName, EventType::Direct, new EventCaller<InputElement, FocusChangedEventArgs>(&InputElement::onEventMouseRootFocusChanged, &InputElement::EventMouseRootFocusChanged));
		registerEvent(gEventKeyboardRootFocusChangedName, EventType::Direct, new EventCaller<InputElement, FocusChangedEventArgs>(&InputElement::onEventKeyboardRootFocusChanged, &InputElement::EventKeyboardRootFocusChanged));

		registerEvent(gEventPreviewMouseMoveName, EventType::Tunnel, new EventCaller<InputElement, MouseMoveEventArgs>(&InputElement::onEventPreviewMouseMove, &InputElement::EventPreviewMouseMove));
		registerEvent(gEventPreviewMouseDragName, EventType::Tunnel, new EventCaller<InputElement, MouseMoveEventArgs>(&InputElement::onEventPreviewMouseDrag, &InputElement::EventPreviewMouseDrag));
		registerEvent(gEventPreviewMouseWheelName, EventType::Tunnel, new EventCaller<InputElement, MouseWheelEventArgs>(&InputElement::onEventPreviewMouseWheel, &InputElement::EventPreviewMouseWheel));
		registerEvent(gEventPreviewMouseButtonDownName, EventType::Tunnel, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventPreviewMouseButtonDown, &InputElement::EventPreviewMouseButtonDown));
		registerEvent(gEventPreviewMouseButtonUpName, EventType::Tunnel, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventPreviewMouseButtonUp, &InputElement::EventPreviewMouseButtonUp));
		registerEvent(gEventPreviewMouseButtonClickName, EventType::Tunnel, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventPreviewMouseButtonClick, &InputElement::EventPreviewMouseButtonClick));
		registerEvent(gEventPreviewMouseButtonDoubleClickName, EventType::Tunnel, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventPreviewMouseButtonDoubleClick, &InputElement::EventPreviewMouseButtonDoubleClick));
		registerEvent(gEventPreviewKeyboardFocusChangedName, EventType::Tunnel, new EventCaller<InputElement, KeyboardFocusChangedEventArgs>(&InputElement::onEventPreviewKeyboardFocusChanged, &InputElement::EventPreviewKeyboardFocusChanged));
		registerEvent(gEventPreviewKeyButtonDownName, EventType::Tunnel, new EventCaller<InputElement, KeyButtonEventArgs>(&InputElement::onEventPreviewKeyButtonDown, &InputElement::EventPreviewKeyButtonDown));
		registerEvent(gEventPreviewKeyButtonUpName, EventType::Tunnel, new EventCaller<InputElement, KeyButtonEventArgs>(&InputElement::onEventPreviewKeyButtonUp, &InputElement::EventPreviewKeyButtonUp));

		registerEvent(gEventMouseMoveName, EventType::Bubble, new EventCaller<InputElement, MouseMoveEventArgs>(&InputElement::onEventMouseMove, &InputElement::EventMouseMove));
		registerEvent(gEventMouseDragName, EventType::Bubble, new EventCaller<InputElement, MouseMoveEventArgs>(&InputElement::onEventMouseDrag, &InputElement::EventMouseDrag));
		registerEvent(gEventMouseWheelName, EventType::Bubble, new EventCaller<InputElement, MouseWheelEventArgs>(&InputElement::onEventMouseWheel, &InputElement::EventMouseWheel));
		registerEvent(gEventMouseButtonDownName, EventType::Bubble, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventMouseButtonDown, &InputElement::EventMouseButtonDown));
		registerEvent(gEventMouseButtonUpName, EventType::Bubble, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventMouseButtonUp, &InputElement::EventMouseButtonUp));
		registerEvent(gEventMouseButtonClickName, EventType::Bubble, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventMouseButtonClick, &InputElement::EventMouseButtonClick));
		registerEvent(gEventMouseButtonDoubleClickName, EventType::Bubble, new EventCaller<InputElement, MouseButtonEventArgs>(&InputElement::onEventMouseButtonDoubleClick, &InputElement::EventMouseButtonDoubleClick));
		registerEvent(gEventKeyboardFocusChangedName, EventType::Bubble, new EventCaller<InputElement, KeyboardFocusChangedEventArgs>(&InputElement::onEventKeyboardFocusChanged, &InputElement::EventKeyboardFocusChanged));
		registerEvent(gEventKeyButtonDownName, EventType::Bubble, new EventCaller<InputElement, KeyButtonEventArgs>(&InputElement::onEventKeyButtonDown, &InputElement::EventKeyButtonDown));
		registerEvent(gEventKeyButtonUpName, EventType::Bubble, new EventCaller<InputElement, KeyButtonEventArgs>(&InputElement::onEventKeyButtonUp, &InputElement::EventKeyButtonUp));
	}

	void InputElement::unregisterInputElement()
	{
		unregisterEvent(gEventMouseFocusChangedName);
		unregisterEvent(gEventMouseRootFocusChangedName);
		unregisterEvent(gEventKeyboardRootFocusChangedName);

		unregisterEvent(gEventPreviewMouseMoveName);
		unregisterEvent(gEventPreviewMouseDragName);
		unregisterEvent(gEventPreviewMouseWheelName);
		unregisterEvent(gEventPreviewMouseButtonDownName);
		unregisterEvent(gEventPreviewMouseButtonUpName);
		unregisterEvent(gEventPreviewMouseButtonClickName);
		unregisterEvent(gEventPreviewMouseButtonDoubleClickName);
		unregisterEvent(gEventPreviewKeyboardFocusChangedName);
		unregisterEvent(gEventPreviewKeyButtonDownName);
		unregisterEvent(gEventPreviewKeyButtonUpName);

		unregisterEvent(gEventMouseMoveName);
		unregisterEvent(gEventMouseDragName);
		unregisterEvent(gEventMouseWheelName);
		unregisterEvent(gEventMouseButtonDownName);
		unregisterEvent(gEventMouseButtonUpName);
		unregisterEvent(gEventMouseButtonClickName);
		unregisterEvent(gEventMouseButtonDoubleClickName);
		unregisterEvent(gEventKeyboardFocusChangedName);
		unregisterEvent(gEventKeyButtonDownName);
		unregisterEvent(gEventKeyButtonUpName);
	}

	void InputElement::raiseEventMouseFocusChanged(FocusChangedEventArgs* _args)
	{
		raiseEvent(gEventMouseFocusChangedName, _args);
	}

	void InputElement::raiseEventMouseRootFocusChanged(FocusChangedEventArgs* _args)
	{
		mMouseRootFocus = _args->getFocus();
		raiseEvent(gEventMouseRootFocusChangedName, _args);
	}

	void InputElement::raiseEventKeyboardRootFocusChanged(FocusChangedEventArgs* _args)
	{
		mKeyboardRootFocus = _args->getFocus();
		raiseEvent(gEventKeyboardRootFocusChangedName, _args);
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

	void InputElement::raiseEventKeyboardFocusChanged(KeyboardFocusChangedEventArgs* _args)
	{
		raiseEvent(gEventPreviewKeyboardFocusChangedName, _args);
		raiseEvent(gEventKeyboardFocusChangedName, _args);
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

	void InputElement::setEnabled(bool _value)
	{
		if (mEnabled == _value)
			return;
		mEnabled = _value;

		if (mInheritsEnabled)
		{
			for (VectorWidgetPtr::iterator iter = static_cast<Widget*>(this)->mWidgetChild.begin(); iter != static_cast<Widget*>(this)->mWidgetChild.end(); ++iter)
				(*iter)->_setInheritsEnable(_value);
			for (VectorWidgetPtr::iterator iter = static_cast<Widget*>(this)->mWidgetChildSkin.begin(); iter !=static_cast<Widget*>(this)-> mWidgetChildSkin.end(); ++iter)
				(*iter)->_setInheritsEnable(_value);

			baseUpdateEnable();
		}

		if (!mEnabled)
		{
			InputManager::getInstance().unlinkWidget(static_cast<Widget*>(this));
		}
	}

	// потом сделать евент конкретный проперти ченджед
	void InputElement::_setInheritsEnable(bool _value)
	{
		if (mInheritsEnabled == _value)
			return;
		mInheritsEnabled = _value;

		if (mEnabled)
		{
			for (VectorWidgetPtr::iterator iter = static_cast<Widget*>(this)->mWidgetChild.begin(); iter != static_cast<Widget*>(this)->mWidgetChild.end(); ++iter)
				(*iter)->_setInheritsEnable(_value);
			for (VectorWidgetPtr::iterator iter = static_cast<Widget*>(this)->mWidgetChildSkin.begin(); iter != static_cast<Widget*>(this)->mWidgetChildSkin.end(); ++iter)
				(*iter)->_setInheritsEnable(_value);

			baseUpdateEnable();
		}

		if (!mEnabled)
		{
			InputManager::getInstance().unlinkWidget(static_cast<Widget*>(this));
		}
	}

	// потом сделать евент конкретный проперти ченджед
	void InputElement::_updateInheritsEnable()
	{
		bool value = false;
		mInheritsEnabled = false;
		for (VectorWidgetPtr::iterator iter = static_cast<Widget*>(this)->mWidgetChild.begin(); iter != static_cast<Widget*>(this)->mWidgetChild.end(); ++iter)
			(*iter)->_setInheritsEnable(value);
		for (VectorWidgetPtr::iterator iter = static_cast<Widget*>(this)->mWidgetChildSkin.begin(); iter != static_cast<Widget*>(this)->mWidgetChildSkin.end(); ++iter)
			(*iter)->_setInheritsEnable(value);
	}

	void InputElement::setVisible(bool _value)
	{
		if (mVisible == _value)
			return;
		mVisible = _value;

		if (mInheritsVisible)
		{
			for (VectorSubWidget::iterator skin = static_cast<Widget*>(this)->mSubSkinChild.begin(); skin != static_cast<Widget*>(this)->mSubSkinChild.end(); ++skin)
				(*skin)->setVisible(_value);
			for (VectorWidgetPtr::iterator widget = static_cast<Widget*>(this)->mWidgetChild.begin(); widget != static_cast<Widget*>(this)->mWidgetChild.end(); ++widget)
				(*widget)->_setInheritsVisible(_value);
			for (VectorWidgetPtr::iterator widget = static_cast<Widget*>(this)->mWidgetChildSkin.begin(); widget != static_cast<Widget*>(this)->mWidgetChildSkin.end(); ++widget)
				(*widget)->_setInheritsVisible(_value);
		}

		// при скрытии скрол баров в скрол вью срывается в рекурсию
		//invalidateMeasure();
	}

	// потом сделать евент конкретный проперти ченджед
	void InputElement::_setInheritsVisible(bool _value)
	{
		if (mInheritsVisible == _value)
			return;
		mInheritsVisible = _value;

		if (mVisible)
		{
			for (VectorSubWidget::iterator skin = static_cast<Widget*>(this)->mSubSkinChild.begin(); skin != static_cast<Widget*>(this)->mSubSkinChild.end(); ++skin)
				(*skin)->setVisible(_value);
			for (VectorWidgetPtr::iterator widget = static_cast<Widget*>(this)->mWidgetChild.begin(); widget != static_cast<Widget*>(this)->mWidgetChild.end(); ++widget)
				(*widget)->_setInheritsVisible(_value);
			for (VectorWidgetPtr::iterator widget = static_cast<Widget*>(this)->mWidgetChildSkin.begin(); widget != static_cast<Widget*>(this)->mWidgetChildSkin.end(); ++widget)
				(*widget)->_setInheritsVisible(_value);
		}
	}

	// потом сделать евент конкретный проперти ченджед
	void InputElement::_updateInheritsVisible()
	{
		bool value = false;
		mInheritsVisible = value;
		for (VectorSubWidget::iterator skin = static_cast<Widget*>(this)->mSubSkinChild.begin(); skin != static_cast<Widget*>(this)->mSubSkinChild.end(); ++skin)
			(*skin)->setVisible(value);
		for (VectorWidgetPtr::iterator widget = static_cast<Widget*>(this)->mWidgetChild.begin(); widget != static_cast<Widget*>(this)->mWidgetChild.end(); ++widget)
			(*widget)->_setInheritsVisible(value);
		for (VectorWidgetPtr::iterator widget = static_cast<Widget*>(this)->mWidgetChildSkin.begin(); widget != static_cast<Widget*>(this)->mWidgetChildSkin.end(); ++widget)
			(*widget)->_setInheritsVisible(value);
	}

} // namespace MyGUI
