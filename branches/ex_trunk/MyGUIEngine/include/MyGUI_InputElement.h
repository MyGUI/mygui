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
#include "MyGUI_FocusChangedEventArgs.h"
#include "MyGUI_KeyboardFocusChangedEventArgs.h"
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

		FocusChangedEventArgsHandler EventMouseFocusChanged;
		FocusChangedEventArgsHandler EventMouseRootFocusChanged;
		FocusChangedEventArgsHandler EventKeyboardRootFocusChanged;

		MouseMoveEventHandler EventPreviewMouseMove;
		MouseMoveEventHandler EventPreviewMouseDrag;
		MouseWheelEventHandler  EventPreviewMouseWheel;
		MouseButtonEventHandler EventPreviewMouseButtonDown;
		MouseButtonEventHandler EventPreviewMouseButtonUp;
		MouseButtonEventHandler EventPreviewMouseButtonClick;
		MouseButtonEventHandler EventPreviewMouseButtonDoubleClick;
		KeyboardFocusChangedEventArgsHandler EventPreviewKeyboardFocusChanged;
		KeyButtonEventHandler EventPreviewKeyButtonDown;
		KeyButtonEventHandler EventPreviewKeyButtonUp;

		MouseMoveEventHandler EventMouseMove;
		MouseMoveEventHandler EventMouseDrag;
		MouseWheelEventHandler  EventMouseWheel;
		MouseButtonEventHandler EventMouseButtonDown;
		MouseButtonEventHandler EventMouseButtonUp;
		MouseButtonEventHandler EventMouseButtonClick;
		MouseButtonEventHandler EventMouseButtonDoubleClick;
		KeyboardFocusChangedEventArgsHandler EventKeyboardFocusChanged;
		KeyButtonEventHandler EventKeyButtonDown;
		KeyButtonEventHandler EventKeyButtonUp;

		/** Set need key focus flag */
		void setNeedKeyFocus(bool _value) { mNeedKeyFocus = _value; }
		/** Is need key focus
			If disable this widget won't be reacting on keyboard at all.\n
			Enabled (true) by default.
		*/
		bool isNeedKeyFocus() const { return mNeedKeyFocus; }

		/** Set need mouse focus flag */
		void setNeedMouseFocus(bool _value) { mNeedMouseFocus = _value; }
		/** Is need mouse focus
			If disable this widget won't be reacting on mouse at all.\n
			Enabled (true) by default.
		*/
		bool isNeedMouseFocus() const { return mNeedMouseFocus; }

		bool getMouseRootFocus() const { return mMouseRootFocus; }

		bool getKeyboardRootFocus() const { return mKeyboardRootFocus; }

		/** Enable or disable widget */
		virtual void setEnabled(bool _value);
		/** Enable or disable widget without changing widget's state */
		void setEnabledSilent(bool _value) { mEnabled = _value; }
		/** Is widget enabled */
		bool isEnabled() const { return mEnabled; }

		/** Hide or show */
		virtual void setVisible(bool _value);
		/** Return true if visible */
		bool isVisible() const { return mVisible; }


	/*internal:*/
		void raiseEventMouseFocusChanged(FocusChangedEventArgs* _args);
		void raiseEventMouseRootFocusChanged(FocusChangedEventArgs* _args);
		void raiseEventKeyboardRootFocusChanged(FocusChangedEventArgs* _args);

		void raiseEventMouseMove(MouseMoveEventArgs* _args);
		void raiseEventMouseDrag(MouseMoveEventArgs* _args);
		void raiseEventMouseWheel(MouseWheelEventArgs* _args);
		void raiseEventMouseButtonDown(MouseButtonEventArgs* _args);
		void raiseEventMouseButtonUp(MouseButtonEventArgs* _args);
		void raiseEventMouseButtonClick(MouseButtonEventArgs* _args);
		void raiseEventMouseButtonDoubleClick(MouseButtonEventArgs* _args);
		void raiseEventKeyboardFocusChanged(KeyboardFocusChangedEventArgs* _args);
		void raiseEventKeyButtonDown(KeyButtonEventArgs* _args);
		void raiseEventKeyButtonUp(KeyButtonEventArgs* _args);

		void _setInheritsEnable(bool _value);
		bool _isInheritsEnable() { return mInheritsEnabled; }
		void _updateInheritsEnable();
		void _setInheritsVisible(bool _value);
		bool _isInheritsVisible() { return mInheritsVisible; }
		void _updateInheritsVisible();

	protected:
		static void registerInputElement();
		static void unregisterInputElement();

		virtual void onEventMouseFocusChanged(Widget* _sender, EventInfo* _info, FocusChangedEventArgs* _args) { }
		virtual void onEventMouseRootFocusChanged(Widget* _sender, EventInfo* _info, FocusChangedEventArgs* _args) { }
		virtual void onEventKeyboardRootFocusChanged(Widget* _sender, EventInfo* _info, FocusChangedEventArgs* _args) { }

		virtual void onEventPreviewMouseMove(Widget* _sender, EventInfo* _info, MouseMoveEventArgs* _args) { }
		virtual void onEventPreviewMouseDrag(Widget* _sender, EventInfo* _info, MouseMoveEventArgs* _args) { }
		virtual void onEventPreviewMouseWheel(Widget* _sender, EventInfo* _info, MouseWheelEventArgs* _args) { }
		virtual void onEventPreviewMouseButtonDown(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args) { }
		virtual void onEventPreviewMouseButtonUp(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args) { }
		virtual void onEventPreviewMouseButtonClick(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args) { }
		virtual void onEventPreviewMouseButtonDoubleClick(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args) { }
		virtual void onEventPreviewKeyboardFocusChanged(Widget* _sender, EventInfo* _info, KeyboardFocusChangedEventArgs* _args) { }
		virtual void onEventPreviewKeyButtonDown(Widget* _sender, EventInfo* _info, KeyButtonEventArgs* _args) { }
		virtual void onEventPreviewKeyButtonUp(Widget* _sender, EventInfo* _info, KeyButtonEventArgs* _args) { }

		virtual void onEventMouseMove(Widget* _sender, EventInfo* _info, MouseMoveEventArgs* _args) { }
		virtual void onEventMouseDrag(Widget* _sender, EventInfo* _info, MouseMoveEventArgs* _args) { }
		virtual void onEventMouseWheel(Widget* _sender, EventInfo* _info, MouseWheelEventArgs* _args) { }
		virtual void onEventMouseButtonDown(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args) { }
		virtual void onEventMouseButtonUp(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args) { }
		virtual void onEventMouseButtonClick(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args) { }
		virtual void onEventMouseButtonDoubleClick(Widget* _sender, EventInfo* _info, MouseButtonEventArgs* _args) { }
		virtual void onEventKeyboardFocusChanged(Widget* _sender, EventInfo* _info, KeyboardFocusChangedEventArgs* _args) { }
		virtual void onEventKeyButtonDown(Widget* _sender, EventInfo* _info, KeyButtonEventArgs* _args) { }
		virtual void onEventKeyButtonUp(Widget* _sender, EventInfo* _info, KeyButtonEventArgs* _args) { }

		virtual void baseUpdateEnable() { }

	private:
		bool mMouseRootFocus;
		bool mKeyboardRootFocus;
		bool mNeedKeyFocus;
		bool mNeedMouseFocus;
		// доступен ли на виджет
		bool mEnabled;
		bool mInheritsEnabled;
		bool mVisible;
		// для иерархического скрытия
		bool mInheritsVisible;
	};

} // namespace MyGUI

#endif // __MYGUI_INPUT_ELEMENT_H__
