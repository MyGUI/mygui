/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*//*
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
#ifndef __MYGUI_WIDGET_EVENT_H__
#define __MYGUI_WIDGET_EVENT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_WidgetToolTip.h"

namespace MyGUI
{

	// делегаты для событий виджета
	typedef delegates::CDelegate1<WidgetPtr> EventHandle_WidgetVoid;
	typedef delegates::CDelegate2<WidgetPtr, WidgetPtr> EventHandle_WidgetWidget;
	typedef delegates::CDelegate2<WidgetPtr, bool> EventHandle_WidgetBool;
	typedef delegates::CDelegate2<WidgetPtr, int> EventHandle_WidgetInt;
	typedef delegates::CDelegate2<WidgetPtr, size_t> EventHandle_WidgetSizeT;
	typedef delegates::CDelegate3<WidgetPtr, int, int> EventHandle_WidgetIntInt;
	typedef delegates::CDelegate4<WidgetPtr, int, int, MouseButton> EventHandle_WidgetIntIntButton;
	typedef delegates::CDelegate2<WidgetPtr, KeyCode> EventHandle_WidgetKeyCode;
	typedef delegates::CDelegate3<WidgetPtr, KeyCode, Char> EventHandle_WidgetKeyCodeChar;
	typedef delegates::CDelegate3<WidgetPtr, const std::string&, const std::string&> EventHandle_WidgetStringString;
	typedef delegates::CDelegate3<WidgetPtr, WidgetPtr&, size_t &> EventHandle_WidgetRefWidgetRefSizeT;
	typedef delegates::CDelegate2<WidgetPtr, const ToolTipInfo & > EventHandle_WidgetToolTip;

	/**
	General information about creating delegate for event :
	@example "Delegate usage"
	@code
		void anyFunc(...) { } // global function

		class AnyClass
		{
		public:
			static void anyStaticMethod(...) { } // static class method
			void anyMethod(...) { } // class method
		};

		AnyClass anyObject; // class instance
	@endcode

	delegate creating:
	@code
		eventAny = MyGUI::newDelegate(anyFunc);
		eventAny = MyGUI::newDelegate(AnyClass::anyStaticMethod);
		eventAny = MyGUI::newDelegate(&anyObject, &AnyClass::anyMethod);
	@endcode
	*/

	class MYGUI_EXPORT WidgetEvent
	{
		friend class InputManager;

    public:
		virtual ~WidgetEvent() { }

	protected:
		WidgetEvent() :
			mWidgetEventSender(0),
			mRootMouseActive(false),
			mRootKeyActive(false)
		{
		}

	public:

		/** Event : Widget lost mouse focus.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)\n
			@param _sender widget that called this event
			@param _new widget with mouse focus or nullptr
		*/
		EventHandle_WidgetWidget eventMouseLostFocus;

		/** Event : Widget got mouse focus.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)\n
			@param _sender widget that called this event
			@param _old widget with mouse focus or nullptr
		*/
		EventHandle_WidgetWidget eventMouseSetFocus;

		/** Event : Widget mouse move with captured widget.\n
			signature : void method(MyGUI::WidgetPtr _sender, int _left, int _top)\n
			@param _sender widget that called this event
			@param _left - pointer position
			@param _top - pointer position
		*/
		EventHandle_WidgetIntInt eventMouseDrag;

		/** Event : Mouse move over widget.\n
			signature : void method(MyGUI::WidgetPtr _sender, int _left, int _top)\n
			@param _sender widget that called this event
			@param _left - pointer position
			@param _top - pointer position
		*/
		EventHandle_WidgetIntInt eventMouseMove;

		/** Event : Mouse wheel over widget.\n
			signature : void method(MyGUI::WidgetPtr _sender, int _rel)\n
			@param _sender widget that called this event
			@param _rel relative wheel position
		*/
		EventHandle_WidgetInt eventMouseWheel;

		/** Event : Mouse button pressed.\n
			signature : void method(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)\n
			@param _sender widget that called this event
			@param _left - pointer position
			@param _top - pointer position
			@param _id Mouse button id
		*/
		EventHandle_WidgetIntIntButton eventMouseButtonPressed;

		/** Event : Mouse button released.\n
			signature : void method(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)\n
			@param _sender widget that called this event
			@param _left - pointer position
			@param _top - pointer position
			@param _id Mouse button id
		*/
		EventHandle_WidgetIntIntButton eventMouseButtonReleased;

		/** Event : Mouse button pressed and released.\n
			signature : void method(MyGUI::WidgetPtr _sender)
			@param _sender widget that called this event
		*/
		EventHandle_WidgetVoid eventMouseButtonClick;

		/** Event : Mouse button double click.\n
			signature : void method(MyGUI::WidgetPtr _sender)
			@param _sender widget that called this event
		*/
		EventHandle_WidgetVoid eventMouseButtonDoubleClick;

		/** Event : Widget lost keyboard focus.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)\n
			@param _sender widget that called this event
			@param _new widget with keyboard focus or nullptr
		*/
		EventHandle_WidgetWidget eventKeyLostFocus;

		/** Event : Widget got keyboard focus.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)\n
			@param _sender widget that called this event
			@param _old widget with keyboard focus or nullptr
		*/
		EventHandle_WidgetWidget eventKeySetFocus;

		/** Event : Key pressed.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key, MyGUI::Char _char)\n
			@param _sender widget that called this event
			@param _key code
			@param _char of pressed symbol (for multilanguage applications)
		*/
		EventHandle_WidgetKeyCodeChar eventKeyButtonPressed;

		/** Event : Key released.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key)\n
			@param _sender widget that called this event
			@param _key code
		*/
		EventHandle_WidgetKeyCode eventKeyButtonReleased;

		/** Event : Root widget changed mouse focus.\n
			info : this event sends only to root widget\n
			signature : void method(MyGUI::WidgetPtr _sender, bool _focus);
			@param _sender widget that called this event
			@param _focus Is widget got mouse focus.
		*/
		EventHandle_WidgetBool  eventRootMouseChangeFocus;

		/** Event : Root widget changed keyboard focus.\n
			info : this event sends only to root widget\n
			signature : void method(MyGUI::WidgetPtr _sender, bool _focus);
			@param _sender widget that called this event
			@param _focus Is widget got keyboard focus.
		*/
		EventHandle_WidgetBool eventRootKeyChangeFocus;

		/** Event : Event about changing tooltip state.\n
			signature : void method(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info);
			@param _sender widget that called this event
			@param _info about tooltip
		*/
		EventHandle_WidgetToolTip eventToolTip;

		/** Event : Extendeble event for special cases or plugins.\n
			signature : void method(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _value);
			@param _sender widget that called this event
			@param _key
			@param _value
		*/
		EventHandle_WidgetStringString eventActionInfo;

		/** Event : Internal request for parent and item index, used for any widget.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr & _container, size_t & _index);
			@param _sender widget that called this event
			@param _container parent
			@param _index of container
		*/
		EventHandle_WidgetRefWidgetRefSizeT _requestGetContainer;

	protected:

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onMouseLostFocus(WidgetPtr _new)
		{
			eventMouseLostFocus(mWidgetEventSender, _new);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onMouseSetFocus(WidgetPtr _old)
		{
			eventMouseSetFocus(mWidgetEventSender, _old);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onMouseDrag(int _left, int _top)
		{
			eventMouseDrag(mWidgetEventSender, _left, _top);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onMouseMove(int _left, int _top)
		{
			eventMouseMove(mWidgetEventSender, _left, _top);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onMouseWheel(int _rel)
		{
			eventMouseWheel(mWidgetEventSender, _rel);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id)
		{
			eventMouseButtonPressed(mWidgetEventSender, _left, _top, _id);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id)
		{
			eventMouseButtonReleased(mWidgetEventSender, _left, _top, _id);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onMouseButtonClick()
		{
			eventMouseButtonClick(mWidgetEventSender);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onMouseButtonDoubleClick()
		{
			eventMouseButtonDoubleClick(mWidgetEventSender);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onKeyLostFocus(WidgetPtr _new)
		{
			eventKeyLostFocus(mWidgetEventSender, _new);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onKeySetFocus(WidgetPtr _old)
		{
			eventKeySetFocus(mWidgetEventSender, _old);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onKeyButtonPressed(KeyCode _key, Char _char)
		{
			eventKeyButtonPressed(mWidgetEventSender, _key, _char);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onKeyButtonReleased(KeyCode _key)
		{
			eventKeyButtonReleased(mWidgetEventSender, _key);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onMouseChangeRootFocus(bool _focus)
		{
			eventRootMouseChangeFocus(mWidgetEventSender, _focus);
		}

		// !!! ОБЯЗАТЕЛЬНО в родительском классе вызывать последним
		virtual void onKeyChangeRootFocus(bool _focus)
		{
			eventRootKeyChangeFocus(mWidgetEventSender, _focus);
		}

		// от чьего имени мы посылаем сообщения
		WidgetPtr mWidgetEventSender;

	private:
		bool mRootMouseActive;
		bool mRootKeyActive;
	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_EVENT_H__
