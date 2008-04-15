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

namespace MyGUI
{
	// делегаты дл€ событий виджета
	typedef delegates::CDelegate1<WidgetPtr> EventInfo_WidgetVoid;
	typedef delegates::CDelegate2<WidgetPtr, WidgetPtr> EventInfo_WidgetWidget;
	typedef delegates::CDelegate2<WidgetPtr, bool> EventInfo_WidgetBool;
	typedef delegates::CDelegate2<WidgetPtr, int> EventInfo_WidgetInt;
	typedef delegates::CDelegate2<WidgetPtr, size_t> EventInfo_WidgetSizeT;
	typedef delegates::CDelegate3<WidgetPtr, int, int> EventInfo_WidgetIntInt;
	typedef delegates::CDelegate3<WidgetPtr, int, Char> EventInfo_WidgetIntChar;
	typedef delegates::CDelegate3<WidgetPtr, const std::string&, const std::string&> EventInfo_WidgetStringString;

	/**
	General information about creating delegate for event :

		void anyFunc(...) {}; // global function

		class AnyClass
		{
		public:
			static void anyStaticMethod(...) {}; // static method
			void anyMethod(...) {}; // обычный метод
		};

		AnyClass anyObject; // class instance

	delegate creating:
		eventAny = MyGUI::newDelegate(anyFunc);
		eventAny = MyGUI::newDelegate(AnyClass::anyStaticMethod);
		eventAny = MyGUI::newDelegate(&anyObject, &AnyClass::anyMethod);
	*/

	class _MyGUIExport WidgetEvent
	{
		friend class InputManager;

    public:
		virtual ~WidgetEvent() {};

	protected:
		WidgetEvent() : mWidgetEventSender(0) {}

	public:

		/*	событие : виджет потер€л фокус мыши*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);*/
		EventInfo_WidgetWidget eventMouseLostFocus;

		/*	событие : виджет получил фокус мыши*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);*/
		EventInfo_WidgetWidget eventMouseSetFocus;

		/*	событие : мышь двигаетс€ над захваченным виджетом*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _left, int _top);*/
		EventInfo_WidgetIntInt eventMouseDrag;

		/*	событие : крутитьс€ колесик над захваченным виджетом*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _rel);*/
		EventInfo_WidgetInt eventMouseWheel;

		/*	событие : нажата клавиша мыши*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _left);*/
		EventInfo_WidgetBool eventMouseButtonPressed;

		/*	событие : отпущенна клавиша мыши*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _left);*/
		EventInfo_WidgetBool eventMouseButtonReleased;

		/*	событие : нажата и отпущенна клавиша мыши*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender);*/
		EventInfo_WidgetVoid eventMouseButtonClick;

		/*	событие : двойной клик*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender);*/
		EventInfo_WidgetVoid eventMouseButtonDoubleClick;

		/*	событие : виджет потер€л фокус клавиатуры*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);*/
		EventInfo_WidgetWidget eventKeyLostFocus;

		/*	событие : виджет получил фокус клавиатуры*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);*/
		EventInfo_WidgetWidget eventKeySetFocus;

		/*	событие : нажата клавиша*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _key, MyGUI::Char _char);*/
		EventInfo_WidgetIntChar eventKeyButtonPressed;

		/*	событие : отпущенна клавиша*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _key);*/
		EventInfo_WidgetInt eventKeyButtonReleased;

		/*	событие : корневой виджет изменил фокус мыши, изменение без дочерних элементов*/
		/* инфо : это событие получает только корневой виджет*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _focus);*/
		EventInfo_WidgetBool  eventRootMouseChangeFocus;

		/*	событие : корневой виджет изменил фокус клавы, изменение без дочерних элементов*/
		/* инфо : это событие получает только корневой виджет*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _focus);*/
		EventInfo_WidgetBool  eventRootKeyChangeFocus;

		/* event : общее расшир€емое событие дл€ плагинов или особых случаев*/
		/* signature : void method(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _value);*/
		EventInfo_WidgetStringString eventActionInfo;

	protected:

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onMouseLostFocus(WidgetPtr _new)
		{
			eventMouseLostFocus(mWidgetEventSender, _new);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onMouseSetFocus(WidgetPtr _old)
		{
			eventMouseSetFocus(mWidgetEventSender, _old);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onMouseDrag(int _left, int _top)
		{
			eventMouseDrag(mWidgetEventSender, _left, _top);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onMouseWheel(int _rel)
		{
			eventMouseWheel(mWidgetEventSender, _rel);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onMouseButtonPressed(bool _left)
		{
			eventMouseButtonPressed(mWidgetEventSender, _left);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onMouseButtonReleased(bool _left)
		{
			eventMouseButtonReleased(mWidgetEventSender, _left);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onMouseButtonClick()
		{
			eventMouseButtonClick(mWidgetEventSender);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onMouseButtonDoubleClick()
		{
			eventMouseButtonDoubleClick(mWidgetEventSender);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onKeyLostFocus(WidgetPtr _new)
		{
			eventKeyLostFocus(mWidgetEventSender, _new);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onKeySetFocus(WidgetPtr _old)
		{
			eventKeySetFocus(mWidgetEventSender, _old);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onKeyButtonPressed(int _key, Char _char)
		{
			eventKeyButtonPressed(mWidgetEventSender, _key, _char);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onKeyButtonReleased(int _key)
		{
			eventKeyButtonReleased(mWidgetEventSender, _key);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onMouseChangeRootFocus(bool _focus)
		{
			eventRootMouseChangeFocus(mWidgetEventSender, _focus);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onKeyChangeRootFocus(bool _focus)
		{
			eventRootKeyChangeFocus(mWidgetEventSender, _focus);
		}

		// от чьего имени мы посылаем сообщени€
		WidgetPtr mWidgetEventSender;
	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_EVENT_H__
