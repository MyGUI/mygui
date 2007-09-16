#ifndef _WIDGETEVENT_H_
#define _WIDGETEVENT_H_

#include "Prerequest.h"
#include "delegate.h"
#include "WidgetDefines.h"

namespace MyGUI
{
	// делегаты дл€ событий виджета
	typedef CDelegate1<WidgetPtr> EventSimple;
	typedef CDelegate2<WidgetPtr, WidgetPtr> EventFocusInfo;
	typedef CDelegate2<WidgetPtr, bool> EventSimpleInfo;
	typedef CDelegate2<WidgetPtr, int> EventSimpleDataInfo;
	typedef CDelegate3<WidgetPtr, int, int> EventCoordInfo;
	typedef CDelegate3<WidgetPtr, int, wchar_t> EventKeyInfo;
	typedef CDelegate3<WidgetPtr, const std::string&, const std::string&> EventActionInfo;

	

	class _MyGUIExport WidgetEvent
	{
		friend InputManager;

	protected:
		WidgetEvent() : m_widgetEventSender(0), m_needKeyFocus(false) {}


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
		//	событие : виджет потер€л фокус мыши
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		EventFocusInfo eventMouseLostFocus;

		//	событие : виджет получил фокус мыши
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		EventFocusInfo eventMouseSetFocus;

		//	событие : мышь двигаетс€ над захваченным виджетом
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _x, int _y);
		EventCoordInfo eventMouseMove;

		//	событие : крутитьс€ колесик над захваченным виджетом
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _rel);
		EventSimpleDataInfo eventMouseSheel;

		//	событие : нажата клавиша мыши
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _left);
		EventSimpleInfo eventMouseButtonPressed;

		//	событие : отпущенна клавиша мыши
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _left);
		EventSimpleInfo eventMouseButtonReleased;

		//	событие : нажата и отпущенна клавиша мыши
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _double);
		EventSimpleInfo eventMouseButtonClick;

		//	событие : виджет потер€л фокус клавиатуры
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		EventFocusInfo eventKeyLostFocus;

		//	событие : виджет получил фокус клавиатуры
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		EventFocusInfo eventKeySetFocus;

		//	событие : нажата клавиша
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _key, wchar_t _char);
		EventKeyInfo eventKeyButtonPressed;

		//	событие : отпущенна клавиша
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _key);
		EventSimpleDataInfo eventKeyButtonReleased;

		//	событие : общее расшир€емое событие дл€ плагинов или особых случаев
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _value);
		EventActionInfo eventActionInfo;

	protected:

		// сообщени€ от менеджера ввода
		virtual void OnMouseLostFocus(WidgetPtr _new)
		{
			eventMouseLostFocus(m_widgetEventSender, _new);
		}

		virtual void OnMouseSetFocus(WidgetPtr _old)
		{
			eventMouseSetFocus(m_widgetEventSender, _old);
		}

		virtual void OnMouseMove(int _x, int _y)
		{
			eventMouseMove(m_widgetEventSender, _x, _y);
		}

		virtual void OnMouseSheel(int _rel)
		{
			eventMouseSheel(m_widgetEventSender, _rel);
		}

		virtual void OnMouseButtonPressed(bool _left)
		{
			eventMouseButtonPressed(m_widgetEventSender, _left);
		}

		virtual void OnMouseButtonReleased(bool _left)
		{
			eventMouseButtonReleased(m_widgetEventSender, _left);
		}

		virtual void OnMouseButtonClick(bool _double)
		{
			eventMouseButtonClick(m_widgetEventSender, _double);
		}

		virtual void OnKeyLostFocus(WidgetPtr _new)
		{
			eventKeyLostFocus(m_widgetEventSender, _new);
		}

		virtual void OnKeySetFocus(WidgetPtr _old)
		{
			eventKeySetFocus(m_widgetEventSender, _old);
		}

		virtual void OnKeyButtonPressed(int _key, wchar_t _char)
		{
			eventKeyButtonPressed(m_widgetEventSender, _key, _char);
		}

		virtual void OnKeyButtonReleased(int _key)
		{
			eventKeyButtonReleased(m_widgetEventSender, _key);
		}

		// нужен ли виджету ввод с клавы
		bool m_needKeyFocus;
		// от чьего имени мы посылаем сообщени€
		WidgetPtr m_widgetEventSender;

	};

} // namespace MyGUI

#endif
