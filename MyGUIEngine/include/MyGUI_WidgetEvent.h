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
	// �������� ��� ������� �������
	typedef delegates::CDelegate1<WidgetPtr> EventInfo_WidgetVoid;
	typedef delegates::CDelegate2<WidgetPtr, WidgetPtr> EventInfo_WidgetWidget;
	typedef delegates::CDelegate2<WidgetPtr, bool> EventInfo_WidgetBool;
	typedef delegates::CDelegate2<WidgetPtr, int> EventInfo_WidgetInt;
	typedef delegates::CDelegate2<WidgetPtr, size_t> EventInfo_WidgetSizeT;
	typedef delegates::CDelegate3<WidgetPtr, int, int> EventInfo_WidgetIntInt;
	typedef delegates::CDelegate3<WidgetPtr, int, wchar_t> EventInfo_WidgetIntWcharT;
	typedef delegates::CDelegate3<WidgetPtr, const std::string&, const std::string&> EventInfo_WidgetStringString;


	class _MyGUIExport WidgetEvent
	{
		friend class InputManager;

    public:
		virtual ~WidgetEvent() {};

	protected:
		WidgetEvent() : mNeedKeyFocus(false), mWidgetEventSender(0) {}


		/*
		����� ���������� �� �������� �������� ��� ������� :

			void anyFunc(...) {}; // ���������� �������

			class AnyClass
			{
			public:
				static void anyStaticMethod(...) {}; // ����������� �����
				void anyMethod(...) {}; // ������� �����
			};

			AnyClass anyObject; // ������ ������

		�������� ��������:
			eventAny = MyGUI::newDelegate(anyFunc);
			eventAny = MyGUI::newDelegate(AnyClass::anyStaticMethod);
			eventAny = MyGUI::newDelegate(&anyObject, &AnyClass::anyMethod);
		*/

	public:

		/*	������� : ������ ������� ����� ����*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);*/
		EventInfo_WidgetWidget eventMouseLostFocus;

		/*	������� : ������ ������� ����� ����*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);*/
		EventInfo_WidgetWidget eventMouseSetFocus;

		/*	������� : ���� ��������� ��� ����������� ��������*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, int _left, int _top);*/
		EventInfo_WidgetIntInt eventMouseMove;

		/*	������� : ��������� ������� ��� ����������� ��������*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, int _rel);*/
		EventInfo_WidgetInt eventMouseWheel;

		/*	������� : ������ ������� ����*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, bool _left);*/
		EventInfo_WidgetBool eventMouseButtonPressed;

		/*	������� : ��������� ������� ����*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, bool _left);*/
		EventInfo_WidgetBool eventMouseButtonReleased;

		/*	������� : ������ � ��������� ������� ����*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, bool _double);*/
		EventInfo_WidgetBool eventMouseButtonClick;

		/*	������� : ������ ������� ����� ����������*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);*/
		EventInfo_WidgetWidget eventKeyLostFocus;

		/*	������� : ������ ������� ����� ����������*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);*/
		EventInfo_WidgetWidget eventKeySetFocus;

		/*	������� : ������ �������*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, int _key, wchar_t _char);*/
		EventInfo_WidgetIntWcharT eventKeyButtonPressed;

		/*	������� : ��������� �������*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, int _key);*/
		EventInfo_WidgetInt eventKeyButtonReleased;

		/*	������� : �������� ������ ������� ����� ����, ��������� ��� �������� ���������*/
		/* ���� : ��� ������� �������� ������ �������� ������*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, bool _focus);*/
		EventInfo_WidgetBool  eventMouseChangeRootFocus;

		/*	������� : �������� ������ ������� ����� �����, ��������� ��� �������� ���������*/
		/* ���� : ��� ������� �������� ������ �������� ������*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, bool _focus);*/
		EventInfo_WidgetBool  eventKeyChangeRootFocus;

		/* event : ����� ����������� ������� ��� �������� ��� ������ �������*/
		/* signature : void method(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _value);*/
		EventInfo_WidgetStringString eventActionInfo;

	protected:

		// !!! ����������� � ������������ ������ �������� ���������
		virtual void _onMouseLostFocus(WidgetPtr _new)
		{
			eventMouseLostFocus(mWidgetEventSender, _new);
		}

		// !!! ����������� � ������������ ������ �������� ���������
		virtual void _onMouseSetFocus(WidgetPtr _old)
		{
			eventMouseSetFocus(mWidgetEventSender, _old);
		}

		// !!! ����������� � ������������ ������ �������� ���������
		virtual void _onMouseMove(int _left, int _top)
		{
			eventMouseMove(mWidgetEventSender, _left, _top);
		}

		// !!! ����������� � ������������ ������ �������� ���������
		virtual void _onMouseWheel(int _rel)
		{
			eventMouseWheel(mWidgetEventSender, _rel);
		}

		// !!! ����������� � ������������ ������ �������� ���������
		virtual void _onMouseButtonPressed(bool _left)
		{
			eventMouseButtonPressed(mWidgetEventSender, _left);
		}

		// !!! ����������� � ������������ ������ �������� ���������
		virtual void _onMouseButtonReleased(bool _left)
		{
			eventMouseButtonReleased(mWidgetEventSender, _left);
		}

		// !!! ����������� � ������������ ������ �������� ���������
		virtual void _onMouseButtonClick(bool _double)
		{
			eventMouseButtonClick(mWidgetEventSender, _double);
		}

		// !!! ����������� � ������������ ������ �������� ���������
		virtual void _onKeyLostFocus(WidgetPtr _new)
		{
			eventKeyLostFocus(mWidgetEventSender, _new);
		}

		// !!! ����������� � ������������ ������ �������� ���������
		virtual void _onKeySetFocus(WidgetPtr _old)
		{
			eventKeySetFocus(mWidgetEventSender, _old);
		}

		// !!! ����������� � ������������ ������ �������� ���������
		virtual void _onKeyButtonPressed(int _key, wchar_t _char)
		{
			eventKeyButtonPressed(mWidgetEventSender, _key, _char);
		}

		// !!! ����������� � ������������ ������ �������� ���������
		virtual void _onKeyButtonReleased(int _key)
		{
			eventKeyButtonReleased(mWidgetEventSender, _key);
		}

		// !!! ����������� � ������������ ������ �������� ���������
		virtual void _onMouseChangeRootFocus(bool _focus)
		{
			eventMouseChangeRootFocus(mWidgetEventSender, _focus);
		}

		// !!! ����������� � ������������ ������ �������� ���������
		virtual void _onKeyChangeRootFocus(bool _focus)
		{
			eventKeyChangeRootFocus(mWidgetEventSender, _focus);
		}

		// ����� �� ������� ���� � �����
		bool mNeedKeyFocus;
		// �� ����� ����� �� �������� ���������
		WidgetPtr mWidgetEventSender;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_EVENT_H__
