#pragma once

#include "delegate.h"
#include "WidgetDefines.h"

namespace widget
{
	// делегаты для событий виджета
	typedef CDelegate1<WidgetPtr> EventSimple;
	typedef CDelegate2<WidgetPtr, WidgetPtr> EventFocusInfo;
	typedef CDelegate2<WidgetPtr, bool> EventSimpleInfo;
	typedef CDelegate2<WidgetPtr, int> EventSimpleDataInfo;
	typedef CDelegate3<WidgetPtr, int, int> EventCoordInfo;
	typedef CDelegate3<WidgetPtr, int, wchar_t> EventKeyInfo;

	class InputManager;

	class WidgetEvent
	{
		friend InputManager;

	protected:
		WidgetEvent();

		/*
		Общая информация по созданию делегата для события :

			void anyFunc(...) {}; // глобальная функция

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
		//	событие : виджет потерял фокус мыши
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		EventFocusInfo eventMouseLostFocus;

		//	событие : виджет получил фокус мыши
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		EventFocusInfo eventMouseSetFocus;

		//	событие : мышь двигается над захваченным виджетом
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _x, int _y);
		EventCoordInfo eventMouseMove;

		//	событие : крутиться колесик над захваченным виджетом
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

		//	событие : виджет потерял фокус клавиатуры
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

	protected:
		// сообщения от менеджера ввода
		virtual void OnMouseLostFocus(WidgetPtr _new);
		virtual void OnMouseSetFocus(WidgetPtr _old);
		virtual void OnMouseMove(int _x, int _y);
		virtual void OnMouseSheel(int _rel);
		virtual void OnMouseButtonPressed(bool _left);
		virtual void OnMouseButtonReleased(bool _left);
		virtual void OnMouseButtonClick(bool _double);

		virtual void OnKeyLostFocus(WidgetPtr _new);
		virtual void OnKeySetFocus(WidgetPtr _old);
		virtual void OnKeyButtonPressed(int _key, wchar_t _char);
		virtual void OnKeyButtonReleased(int _key);

		// нужен ли виджету ввод с клавы
		bool m_needKeyFocus;
		// от чьего имени мы посылаем сообщения
		WidgetPtr m_widgetEventSender;

	};

} // namespace widget