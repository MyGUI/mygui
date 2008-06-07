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
		enum ViewValueInfo
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
			CancelTryContinue = Cancel | Try | Continue,

			IconDefault = FLAG(16),

			IconInfo = FLAG(16),
			IconQuest = FLAG(17),
			IconError = FLAG(18),
			IconWarning = FLAG(19),

			Icon1 = FLAG(16),
			Icon2 = FLAG(17),
			Icon3 = FLAG(18),
			Icon4 = FLAG(19),
			Icon5 = FLAG(20),
			Icon6 = FLAG(21),
			Icon7 = FLAG(22),
			Icon8 = FLAG(23),
		};
		typedef size_t ViewInfo;
		typedef delegates::CDelegate2<WidgetPtr, ViewInfo> EventInfo_WidgetMessage;
		typedef delegates::IDelegate2<WidgetPtr, ViewInfo> EventMessageEnd;

	protected:
		Message(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);

		void updateSize();
		void notifyButtonClick(MyGUI::WidgetPtr _sender);
		void clearButton();

		void _onKeyButtonPressed(KeyCode _key, Char _char);
		void _destroyMessage(ViewInfo _result);

		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		/** Set message text*/
		void setMessage(const Ogre::UTFString & _message);

		/** Create button with specific name*/
		Message::ViewInfo addButtonName(const Ogre::UTFString & _name);

		/** Create button using ViewValueInfo*/
		void setButton(ViewInfo _info);

		/** Set smooth message showing*/
		void setSmoothShow(bool _smooth);

		/** Get name of default layer for Message*/
		inline const std::string & getDefaultLayer() {return mDefaultLayer;}

		/** Set message image*/
		void setMessageImage(size_t _image);
		/** Set fade under message*/
		void setWindowFade(bool _fade);

		/** Static method for creating message with one command
			@param
				_modal if true all other GUI elements will be blocked untill message is closed
			@param
				_delegate event called on message close. void method(MyGUI::WidgetPtr _sender, MyGUI::Message::ViewInfo _button);
			@param
				_info any combination of flags from ViewValueInfo
			@param
				_button1 ... _button7 specific buttons names
		*/
		static MyGUI::MessagePtr _createMessage(const Ogre::UTFString & _caption, const Ogre::UTFString & _message, 
			const std::string & _skin, const std::string & _layer, bool _modal, EventMessageEnd * _delegate, ViewInfo _info,
			const std::string & _button1 = "", const std::string & _button2 = "", const std::string & _button3 = "", const std::string & _button4 = "",
			const std::string & _button5 = "", const std::string & _button6 = "", const std::string & _button7 = "");

		/** See Message::_createMessage*/
		inline static MyGUI::MessagePtr createMessage(const Ogre::UTFString & _caption, const Ogre::UTFString & _message, bool _modal, ViewInfo _info)
		{
			return _createMessage(_caption, _message, "", "", _modal, null, _info);
		}

		/** See Message::_createMessage*/
		inline static MyGUI::MessagePtr createMessage(const Ogre::UTFString & _caption, const Ogre::UTFString & _message, bool _modal, EventMessageEnd * _delegate, ViewInfo _info)
		{
			return _createMessage(_caption, _message, "", "", _modal, _delegate, _info);
		}

		/** See Message::_createMessage*/
		inline static MyGUI::MessagePtr createMessage(const Ogre::UTFString & _caption, const Ogre::UTFString & _message, bool _modal,
			const std::string & _button1, const std::string & _button2 = "", const std::string & _button3 = "")
		{
			return _createMessage(_caption, _message, "", "", _modal, null, None, _button1, _button2, _button3);
		}

		/** See Message::_createMessage*/
		inline static MyGUI::MessagePtr createMessage(const Ogre::UTFString & _caption, const Ogre::UTFString & _message, bool _modal,
			EventMessageEnd * _delegate, const std::string & _button1, const std::string & _button2 = "", const std::string & _button3 = "")
		{
			return _createMessage(_caption, _message, "", "", _modal, _delegate, None, _button1, _button2, _button3);
		}

		/** Event : button on message window pressed.\n
			signature : void method(MyGUI::WidgetPtr _sender, MyGUI::Message::ViewInfo _button)\n
			@param _button - id of pressed button
		*/
		EventInfo_WidgetMessage eventMessageBoxEnd;

	private:
		IntSize mOffsetText;
		WidgetPtr mWidgetText;

		std::string mButtonSkin, mButtonType;
		IntSize mButtonSize, mButtonOffset;

		VectorWidgetPtr mVectorButton;
		ViewInfo mInfoOk, mInfoCancel;
		size_t mButton1Index;
		bool mSmoothShow;

		std::string mDefaultLayer, mDefaultCaption;
		std::string mFadeSkin, mFadeLayer;
		WidgetPtr mWidgetFade;
		StaticImagePtr mIcon;

	}; // class _MyGUIExport Message : public Window

} // namespace MyGUI

#endif // __MYGUI_MESSAGE_H__
