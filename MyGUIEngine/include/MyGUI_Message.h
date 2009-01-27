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
#include "MyGUI_ResourceImageSet.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_MessageStyle.h"

namespace MyGUI
{

	typedef delegates::CDelegate2<MessagePtr, MessageStyle> EventHandle_MessagePtrMessageStyle;

	class MYGUI_EXPORT Message : public Window
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<Message>;

		MYGUI_RTTI_CHILD_HEADER( Message, Window );

	public:
		/** Set message text*/
		void setMessage(const Ogre::UTFString & _message);

		/** Create button with specific name*/
		MessageStyle addButtonName(const Ogre::UTFString & _name);

		/** Set smooth message showing*/
		void setSmoothShow(bool _smooth);

		/** Get name of default layer for Message*/
		const std::string & getDefaultLayer() { return mDefaultLayer; }

		/** Set message image*/
		void setMessageImage(MessageStyle _image);
		/** Set fade under message*/
		void setWindowFade(bool _fade);

		void endMessage(MessageStyle _result) { _destroyMessage(_result); }
		void endMessage() { _destroyMessage(mInfoCancel); }

		/** Create button using MessageStyle*/
		void setMessageButton(MessageStyle _button);

		/** Static method for creating message with one command
			@param
				_modal if true all other GUI elements will be blocked untill message is closed
			@param
				_style any combination of flags from ViewValueInfo
			@param
				_button1 ... _button4 specific buttons names
		*/
		static MyGUI::MessagePtr createMessageBox(
			const std::string & _skin,
			const Ogre::UTFString & _caption,
			const Ogre::UTFString & _message,
			MessageStyle _style = MessageStyle::Ok | MessageStyle::IconDefault,
			const std::string & _layer = "",
			bool _modal = true,
			const std::string & _button1 = "",
			const std::string & _button2 = "",
			const std::string & _button3 = "",
			const std::string & _button4 = "");

	/*event:*/
		/** Event : button on message window pressed.\n
			signature : void method(MyGUI::MessagePtr _sender, MyGUI::MessageStyle _result)\n
			@param _sender widget that called this event
			@param _result - id of pressed button
		*/
		EventHandle_MessagePtrMessageStyle eventMessageBoxResult;

	protected:
		Message(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~Message();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

		void updateSize();
		void notifyButtonClick(MyGUI::WidgetPtr _sender);
		void clearButton();

		void onKeyButtonPressed(KeyCode _key, Char _char);
		void _destroyMessage(MessageStyle _result);

		const char * getIconName(size_t _index);

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

	private:
		IntSize mOffsetText;
		WidgetPtr mWidgetText;

		std::string mButtonSkin, mButtonType;
		IntSize mButtonSize, mButtonOffset;

		VectorWidgetPtr mVectorButton;
		MessageStyle mInfoOk;
		MessageStyle mInfoCancel;
		bool mSmoothShow;

		std::string mDefaultLayer, mDefaultCaption;
		std::string mFadeSkin, mFadeLayer;
		WidgetPtr mWidgetFade;
		StaticImagePtr mIcon;
		int mLeftOffset1;
		int mLeftOffset2;

	};

} // namespace MyGUI

#endif // __MYGUI_MESSAGE_H__
