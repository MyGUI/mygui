/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_EVENT_H__
#define __MYGUI_WIDGET_EVENT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	// делегаты дл€ событий виджета
	typedef delegates::CDelegate1<WidgetPtr> EventSimple;
	typedef delegates::CDelegate2<WidgetPtr, WidgetPtr> EventFocusInfo;
	typedef delegates::CDelegate2<WidgetPtr, bool> EventSimpleInfo;
	typedef delegates::CDelegate2<WidgetPtr, int> EventSimpleDataInfo;
	typedef delegates::CDelegate3<WidgetPtr, int, int> EventCoordInfo;
	typedef delegates::CDelegate3<WidgetPtr, int, wchar_t> EventKeyInfo;
	typedef delegates::CDelegate3<WidgetPtr, const std::string&, const std::string&> EventActionInfo;
	

	class _MyGUIExport WidgetEvent
	{
		friend InputManager;

	protected:
		WidgetEvent() : mWidgetEventSender(0), mNeedKeyFocus(false) {}


		/*
		ќбща€ информаци€ по созданию делегата дл€ событи€ :

			void anyFunc(...) {}; // глобальна€ функци€

			class AnyClass
			{
			public:
				static void anyStaticMethod(...) {}; // статический метод
				void anyMethod(...) {}; // обычный метод
			};
			
			AnyClass anyObject; // объект класса

		создание делегата:
			eventAny = MyGUI::newDelegate(anyFunc);
			eventAny = MyGUI::newDelegate(AnyClass::anyStaticMethod);
			eventAny = MyGUI::newDelegate(&anyObject, &AnyClass::anyMethod);
		*/

	public:

		/*	событие : виджет потер€л фокус мыши*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);*/
		EventFocusInfo eventMouseLostFocus;

		/*	событие : виджет получил фокус мыши*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);*/
		EventFocusInfo eventMouseSetFocus;

		/*	событие : мышь двигаетс€ над захваченным виджетом*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _left, int _top);*/
		EventCoordInfo eventMouseMove;

		/*	событие : крутитьс€ колесик над захваченным виджетом*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _rel);*/
		EventSimpleDataInfo eventMouseWheel;

		/*	событие : нажата клавиша мыши*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _left);*/
		EventSimpleInfo eventMouseButtonPressed;

		/*	событие : отпущенна клавиша мыши*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _left);*/
		EventSimpleInfo eventMouseButtonReleased;

		/*	событие : нажата и отпущенна клавиша мыши*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _double);*/
		EventSimpleInfo eventMouseButtonClick;

		/*	событие : виджет потер€л фокус клавиатуры*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);*/
		EventFocusInfo eventKeyLostFocus;

		/*	событие : виджет получил фокус клавиатуры*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);*/
		EventFocusInfo eventKeySetFocus;

		/*	событие : нажата клавиша*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _key, wchar_t _char);*/
		EventKeyInfo eventKeyButtonPressed;

		/*	событие : отпущенна клавиша*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _key);*/
		EventSimpleDataInfo eventKeyButtonReleased;

		/*	событие : корневой виджет изменил фокус мыши, изменение без дочерних элементов*/
		/* инфо : это событие получает только корневой виджет*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _focus);*/
		EventSimpleInfo  eventMouseChangeRootFocus;

		/*	событие : корневой виджет изменил фокус клавы, изменение без дочерних элементов*/
		/* инфо : это событие получает только корневой виджет*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _focus);*/
		EventSimpleInfo  eventKeyChangeRootFocus;

		/* event : общее расшир€емое событие дл€ плагинов или особых случаев*/
		/* signature : void method(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _value);*/
		EventActionInfo eventActionInfo;

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
		virtual void _onMouseMove(int _left, int _top)
		{
			eventMouseMove(mWidgetEventSender, _left, _top);
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
		virtual void _onMouseButtonClick(bool _double)
		{
			eventMouseButtonClick(mWidgetEventSender, _double);
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
		virtual void _onKeyButtonPressed(int _key, wchar_t _char)
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
			eventMouseChangeRootFocus(mWidgetEventSender, _focus);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ в родительском классе вызывать последним
		virtual void _onKeyChangeRootFocus(bool _focus)
		{
			eventKeyChangeRootFocus(mWidgetEventSender, _focus);
		}

		// нужен ли виджету ввод с клавы
		bool mNeedKeyFocus;
		// от чьего имени мы посылаем сообщени€
		WidgetPtr mWidgetEventSender;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_EVENT_H__