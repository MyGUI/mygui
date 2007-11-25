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
	// делегаты для событий виджета
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
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, int _left, int _top);
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

		//	событие : корневой виджет изменил фокус мыши, изменение без дочерних элементов
		// это событие получает только корневой виджет
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _focus);
		EventSimpleInfo  eventMouseChangeRootFocus;

		//	событие : корневой виджет изменил фокус клавы, изменение без дочерних элементов
		// это событие получает только корневой виджет
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, bool _focus);
		EventSimpleInfo  eventKeyChangeRootFocus;

		//	событие : общее расширяемое событие для плагинов или особых случаев
		//	прототип делегата : void method(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _value);
		EventActionInfo eventActionInfo;

	protected:

		// сообщения от менеджера ввода
		virtual void _onMouseLostFocus(WidgetPtr _new)
		{
			eventMouseLostFocus(mWidgetEventSender, _new);
		}

		virtual void _onMouseSetFocus(WidgetPtr _old)
		{
			eventMouseSetFocus(mWidgetEventSender, _old);
		}

		virtual void _onMouseMove(int _left, int _top)
		{
			eventMouseMove(mWidgetEventSender, _left, _top);
		}

		virtual void _onMouseSheel(int _rel)
		{
			eventMouseSheel(mWidgetEventSender, _rel);
		}

		virtual void _onMouseButtonPressed(bool _left)
		{
			eventMouseButtonPressed(mWidgetEventSender, _left);
		}

		virtual void _onMouseButtonReleased(bool _left)
		{
			eventMouseButtonReleased(mWidgetEventSender, _left);
		}

		virtual void _onMouseButtonClick(bool _double)
		{
			eventMouseButtonClick(mWidgetEventSender, _double);
		}

		virtual void _onKeyLostFocus(WidgetPtr _new)
		{
			eventKeyLostFocus(mWidgetEventSender, _new);
		}

		virtual void _onKeySetFocus(WidgetPtr _old)
		{
			eventKeySetFocus(mWidgetEventSender, _old);
		}

		virtual void _onKeyButtonPressed(int _key, wchar_t _char)
		{
			eventKeyButtonPressed(mWidgetEventSender, _key, _char);
		}

		virtual void _onKeyButtonReleased(int _key)
		{
			eventKeyButtonReleased(mWidgetEventSender, _key);
		}

		virtual void _onMouseChangeRootFocus(bool _focus)
		{
			eventMouseChangeRootFocus(mWidgetEventSender, _focus);
		}

		virtual void _onKeyChangeRootFocus(bool _focus)
		{
			eventKeyChangeRootFocus(mWidgetEventSender, _focus);
		}

		// нужен ли виджету ввод с клавы
		bool mNeedKeyFocus;
		// от чьего имени мы посылаем сообщения
		WidgetPtr mWidgetEventSender;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_EVENT_H__