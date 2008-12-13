/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
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
	typedef delegates::CDelegate1<WidgetPtr> EventInfo_WidgetVoid;
	typedef delegates::CDelegate2<WidgetPtr, WidgetPtr> EventInfo_WidgetWidget;
	typedef delegates::CDelegate2<WidgetPtr, bool> EventInfo_WidgetBool;
	typedef delegates::CDelegate2<WidgetPtr, int> EventInfo_WidgetInt;
	typedef delegates::CDelegate2<WidgetPtr, size_t> EventInfo_WidgetSizeT;
	typedef delegates::CDelegate3<WidgetPtr, int, int> EventInfo_WidgetIntInt;
	typedef delegates::CDelegate4<WidgetPtr, int, int, MouseButton> EventInfo_WidgetIntIntButton;
	typedef delegates::CDelegate2<WidgetPtr, KeyCode> EventInfo_WidgetKeyCode;
	typedef delegates::CDelegate3<WidgetPtr, KeyCode, Char> EventInfo_WidgetKeyCodeChar;
	typedef delegates::CDelegate3<WidgetPtr, const std::string&, const std::string&> EventInfo_WidgetStringString;
	typedef delegates::CDelegate3<WidgetPtr, WidgetPtr&, size_t &> EventInfo_WidgetRefWidgetRefSizeT;
	typedef delegates::CDelegate2<WidgetPtr, const ToolTipInfo & > EventInfo_WidgetToolTip;

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
			@param _new widget with mouse focus or null
		*/
		EventInfo_WidgetWidget eventMouseLostFocus;

		/** Event : Widget got mouse focus.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)\n
			@param _old widget with mouse focus or null
		*/
		EventInfo_WidgetWidget eventMouseSetFocus;

		/** Event : Widget mouse move with captured widget.\n
			signature : void method(MyGUI::WidgetPtr _sender, int _left, int _top)\n
			@param _left, _top - pointer position
		*/
		EventInfo_WidgetIntInt eventMouseDrag;

		/** Event : Mouse move over widget.\n
			signature : void method(MyGUI::WidgetPtr _sender, int _left, int _top)\n
			@param _left, _top - pointer position
		*/
		EventInfo_WidgetIntInt eventMouseMove;

		/** Event : Mouse wheel over widget.\n
			signature : void method(MyGUI::WidgetPtr _sender, int _rel)\n
			@param _rel relative wheel position
		*/
		EventInfo_WidgetInt eventMouseWheel;

		/** Event : Mouse button pressed.\n
			signature : void method(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)\n
			@param _left, _top pointer position
			@param _id Mouse button id
		*/
		EventInfo_WidgetIntIntButton eventMouseButtonPressed;

		/** Event : Mouse button released.\n
			signature : void method(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)\n
			@param _left, _top pointer position
			@param _id Mouse button id
		*/
		EventInfo_WidgetIntIntButton eventMouseButtonReleased;

		/** Event : Mouse button pressed and released.\n
			signature : void method(MyGUI::WidgetPtr _sender)
		*/
		EventInfo_WidgetVoid eventMouseButtonClick;

		/** Event : Mouse button double click.\n
			signature : void method(MyGUI::WidgetPtr _sender)
		*/
		EventInfo_WidgetVoid eventMouseButtonDoubleClick;

		/** Event : Widget lost keyboard focus.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)\n
			@param _new widget with keyboard focus or null
		*/
		EventInfo_WidgetWidget eventKeyLostFocus;

		/** Event : Widget got keyboard focus.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)\n
			@param _old widget with keyboard focus or null
		*/
		EventInfo_WidgetWidget eventKeySetFocus;

		/** Event : Key pressed.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key, MyGUI::Char _char)\n
			@param _key code
			@param _char of pressed symbol (for multilanguage applications)
		*/
		EventInfo_WidgetKeyCodeChar eventKeyButtonPressed;

		/** Event : Key released.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key)\n
			@param _key code
		*/
		EventInfo_WidgetKeyCode eventKeyButtonReleased;

		/** Event : Root widget changed mouse focus.\n
			info : this event sends only to root widget\n
			signature : void method(MyGUI::WidgetPtr _sender, bool _focus);
			@param _focus Is widget got mouse focus.
		*/
		EventInfo_WidgetBool  eventRootMouseChangeFocus;

		/** Event : Root widget changed keyboard focus.\n
			info : this event sends only to root widget\n
			signature : void method(MyGUI::WidgetPtr _sender, bool _focus);
			@param _focus Is widget got keyboard focus.
		*/
		EventInfo_WidgetBool eventRootKeyChangeFocus;

		/** Event : Extendeble event for special cases or plugins.\n
			signature : void method(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _value);
		*/
		EventInfo_WidgetStringString eventActionInfo;

		/** Event : Internal request for parent and item index, used for any widget.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr & _container, size_t & _index);
			@param _container parent
			@param _index of widget
		*/
		EventInfo_WidgetRefWidgetRefSizeT requestGetContainer;

		/** Event : Event about changing tooltip state.\n
			signature : void method(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info);
			@param _info about tooltip
		*/
		EventInfo_WidgetToolTip eventToolTip;

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
