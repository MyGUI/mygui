/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_MESSAGE_H__
#define __MYGUI_MESSAGE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Window.h"

namespace MyGUI
{

	class _MyGUIExport Message : public Window
	{
		// ��� ������ ��������� ������������
		friend class factory::MessageFactory;

	public:
		enum ButtonValueInfo
		{
			None = FLAG_NONE,
			Ok = FLAG(0),
			Yes = FLAG(1),
			No = FLAG(2),
			Abort = FLAG(3),
			Retry = FLAG(4),
			Ignore = FLAG(5),
			Cancel = FLAG(6),
			Try = FLAG(7),
			Continue = FLAG(8),

			Button1 = FLAG(9),
			Button2 = FLAG(10),
			Button3 = FLAG(11),
			Button4 = FLAG(12),
			Button5 = FLAG(13),
			Button6 = FLAG(14),
			Button7 = FLAG(15),

			OkCancel = Ok | Cancel,
			YesNo = Yes | No,
			YesNoCancel = Yes | No | Cancel,
			RetryCancel = Retry | Cancel,
			AbortRetryIgnore = Abort | Retry | Ignore,
			CancelTryContinue = Cancel | Try | Continue
		};
		typedef unsigned short ButtonInfo;
		typedef delegates::CDelegate2<WidgetPtr, ButtonInfo> EventInfo_WidgetMessage;

	protected:
		Message(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

		void updateSize();
		void notifyButtonClick(MyGUI::WidgetPtr _sender, bool _double);
		void clearButton();

		void _onKeyButtonPressed(int _key, Char _char);

	public:
		// ��� ������� �������
		inline static const Ogre::String & _getType() {static Ogre::String type("Message"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		// ��������� ������ ���������
		void setMessage(const Ogre::DisplayString & _message);

		// �������� ������ �� ����� ������
		Message::ButtonInfo addButtonName(const Ogre::DisplayString & _name);

		// �������� ������ �� ������� �����
		void setButton(ButtonInfo _info);

	public:
		/*	������� : ������ ������ �� ���� ���������*/
		/*	�������� �������� : void method(MyGUI::WidgetPtr _sender, MyGUI::ButtonInfo _button);*/
		EventInfo_WidgetMessage eventMessageBoxEnd;

	private:
		IntSize mOffsetText;
		WidgetPtr mWidgetText;

		std::string mButtonSkin, mButtonType;
		IntSize mButtonSize, mButtonOffset;

		VectorWidgetPtr mVectorButton;
		ButtonInfo mInfoOk, mInfoCancel;
		size_t mButton1Index;

	}; // class _MyGUIExport Message : public Window

} // namespace MyGUI

#endif // __MYGUI_MESSAGE_H__
