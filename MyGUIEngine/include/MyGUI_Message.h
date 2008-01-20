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
		// для вызова закрытого конструктора
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
		typedef delegates::IDelegate2<WidgetPtr, ButtonInfo> EventMessageEnd;

	protected:
		Message(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

		void updateSize();
		void notifyButtonClick(MyGUI::WidgetPtr _sender, bool _double);
		void clearButton();

		void _onKeyButtonPressed(int _key, Char _char);

	public:
		// тип данного виджета
		inline static const Ogre::String & _getType() {static Ogre::String type("Message"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		// установка текста сообщения
		void setMessage(const Ogre::DisplayString & _message);

		// создание кнопки со своим именем
		Message::ButtonInfo addButtonName(const Ogre::DisplayString & _name);

		// создание кнопок по битовым полям
		void setButton(ButtonInfo _info);

		void setWindowSmooth(bool _smooth);

		inline const std::string & getDefaultLayer() {return mDefaultLayer;}
		void setMessageImage(size_t _image);

		static void _createMessage(const Ogre::DisplayString & _caption, const Ogre::DisplayString & _message, size_t _image,
			const std::string & _skin, const std::string & _layer, bool _modal, EventMessageEnd * _delegate, ButtonInfo _info,
			const std::string & _button1 = "", const std::string & _button2 = "", const std::string & _button3 = "", const std::string & _button4 = "",
			const std::string & _button5 = "", const std::string & _button6 = "", const std::string & _button7 = "");

		inline static void createMessage(const Ogre::DisplayString & _caption, const Ogre::DisplayString & _message, bool _modal, ButtonInfo _info)
		{
			_createMessage(_caption, _message, ITEM_NONE, "", "", _modal, null, _info);
		}

		inline static void createMessage(const Ogre::DisplayString & _caption, const Ogre::DisplayString & _message, bool _modal, EventMessageEnd * _delegate, ButtonInfo _info)
		{
			_createMessage(_caption, _message, ITEM_NONE, "", "", _modal, _delegate, _info);
		}

		inline static void createMessage(const Ogre::DisplayString & _caption, const Ogre::DisplayString & _message, bool _modal,
			const std::string & _button1, const std::string & _button2 = "", const std::string & _button3 = "")
		{
			_createMessage(_caption, _message, ITEM_NONE, "", "", _modal, null, None, _button1, _button2, _button3);
		}

		inline static void createMessage(const Ogre::DisplayString & _caption, const Ogre::DisplayString & _message, bool _modal,
			EventMessageEnd * _delegate, const std::string & _button1, const std::string & _button2 = "", const std::string & _button3 = "")
		{
			_createMessage(_caption, _message, ITEM_NONE, "", "", _modal, _delegate, None, _button1, _button2, _button3);
		}

	public:
		/*	событие : надата кнопка на окне сообщений*/
		/*	прототип делегата : void method(MyGUI::WidgetPtr _sender, MyGUI::ButtonInfo _button);*/
		EventInfo_WidgetMessage eventMessageBoxEnd;

	private:
		IntSize mOffsetText;
		WidgetPtr mWidgetText;

		std::string mButtonSkin, mButtonType;
		IntSize mButtonSize, mButtonOffset;

		VectorWidgetPtr mVectorButton;
		ButtonInfo mInfoOk, mInfoCancel;
		size_t mButton1Index;
		bool mSmooth;

		std::string mDefaultLayer;
		std::string mDefaultCaption;
	}; // class _MyGUIExport Message : public Window

} // namespace MyGUI

#endif // __MYGUI_MESSAGE_H__
