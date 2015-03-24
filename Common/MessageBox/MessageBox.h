/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef MESSAGE_BOX_H_
#define MESSAGE_BOX_H_

#include <MyGUI.h>
#include "MessageBoxStyle.h"
#include "BaseLayout/BaseLayout.h"

namespace MyGUI
{
	class Message;

	typedef delegates::CMultiDelegate2<Message*, MessageBoxStyle> EventHandle_MessageBoxPtrMessageStyle;

	class Message :
		public wraps::BaseLayout
	{
	public:
		Message() :
			wraps::BaseLayout("MessageBox.layout"),
			mWidgetText(nullptr),
			mInfoOk(MessageBoxStyle::None),
			mInfoCancel(MessageBoxStyle::None),
			mSmoothShow(false),
			mIcon(nullptr),
			mLeftOffset1(0),
			mLeftOffset2(0)
		{
			initialise();
		}

		Message(const std::string& _layoutName) :
			wraps::BaseLayout(_layoutName),
			mWidgetText(nullptr),
			mInfoOk(MessageBoxStyle::None),
			mInfoCancel(MessageBoxStyle::None),
			mSmoothShow(false),
			mIcon(nullptr),
			mLeftOffset1(0),
			mLeftOffset2(0)
		{
			initialise();
		}

		virtual ~Message()
		{
			mWidgetText = nullptr;
			mIcon = nullptr;
		}

		/** Set caption text*/
		void setCaption(const UString& _value)
		{
			mMainWidget->castType<Window>()->setCaption(_value);
		}

		/** Set message text*/
		void setMessageText(const UString& _value)
		{
			if (mWidgetText != nullptr)
				mWidgetText->setCaption(_value);
			updateSize();
		}

		/** Create button with specific name*/
		MessageBoxStyle addButtonName(const UString& _name)
		{
			if (mVectorButton.size() >= MessageBoxStyle::_CountUserButtons)
			{
				MYGUI_LOG(Warning, "Too many buttons in message box, ignored");
				return MessageBoxStyle::None;
			}
			// бит, номер кнопки + смещение до Button1
			MessageBoxStyle info = MessageBoxStyle(MessageBoxStyle::Enum(MYGUI_FLAG(mVectorButton.size() + MessageBoxStyle::_IndexUserButton1)));

			// запоминаем кнопки для отмены и подтверждения
			if (mVectorButton.empty())
				mInfoOk = info;
			mInfoCancel = info;

			Widget* widget = mMainWidget->createWidgetT(mButtonType, mButtonSkin, IntCoord(), Align::Left | Align::Bottom);
			Button* button = widget->castType<Button>();
			button->eventMouseButtonClick += newDelegate(this, &Message::notifyButtonClick);
			button->setCaption(_name);
			button->_setInternalData(info);
			mVectorButton.push_back(button);

			updateSize();
			return info;
		}

		/** Set smooth message showing*/
		void setSmoothShow(bool _value)
		{
			mSmoothShow = _value;
			if (mSmoothShow)
			{
				mMainWidget->setAlpha(ALPHA_MIN);
				mMainWidget->setVisible(true);
				mMainWidget->castType<Window>()->setVisibleSmooth(true);
			}
		}

		/** Set message icon*/
		void setMessageIcon(MessageBoxStyle _value)
		{
			if (nullptr == mIcon)
				return;

			if (mIcon->getItemResource() != nullptr)
			{
				mIcon->setItemName(getIconName(_value.getIconIndex()));
			}
			else
			{
				mIcon->setImageIndex(_value.getIconIndex());
			}

			updateSize();
		}

		void endMessage(MessageBoxStyle _result)
		{
			_destroyMessage(_result);
		}

		void endMessage()
		{
			_destroyMessage(mInfoCancel);
		}

		/** Create button using MessageBoxStyle*/
		void setMessageButton(MessageBoxStyle _value)
		{
			clearButton();

			std::vector<MessageBoxStyle> buttons = _value.getButtons();

			for (size_t index = 0; index < buttons.size(); ++index)
			{
				// корректируем ее номер
				MessageBoxStyle info = buttons[index];

				// если бит есть то ставим кнопку
				addButtonName(getButtonName(info));

				// внутри адд сбрасывается
				mVectorButton.back()->_setInternalData(info);

				// первая кнопка
				if (mVectorButton.size() == 1)
					mInfoOk = info;
				// последняя кнопка
				mInfoCancel = info;
			}

			updateSize();
		}

		/** Set message style (button and icon)*/
		void setMessageStyle(MessageBoxStyle _value)
		{
			setMessageButton(_value);
			setMessageIcon(_value);
		}

		void setMessageModal(bool _value)
		{
			if (_value)
				InputManager::getInstance().addWidgetModal(mMainWidget);
			else
				InputManager::getInstance().removeWidgetModal(mMainWidget);
		}

		/** Static method for creating message with one command
			@param
				_modal if true all other GUI elements will be blocked untill message is closed
			@param
				_style any combination of flags from ViewValueInfo
			@param
				_button1 ... _button4 specific buttons names
		*/
		static Message* createMessageBox(
			//const UString& _skinName,
			const UString& _caption,
			const UString& _message,
			MessageBoxStyle _style = MessageBoxStyle::Ok | MessageBoxStyle::IconDefault,
			const std::string& _layer = "",
			bool _modal = true,
			const std::string& _button1 = "",
			const std::string& _button2 = "",
			const std::string& _button3 = "",
			const std::string& _button4 = "")
		{
			Message* mess = new Message();

			mess->setCaption(_caption);
			mess->setMessageText(_message);

			mess->setSmoothShow(true);

			mess->setMessageStyle(_style);

			if (!_button1.empty())
			{
				mess->addButtonName(_button1);
				if (!_button2.empty())
				{
					mess->addButtonName(_button2);
					if (!_button3.empty())
					{
						mess->addButtonName(_button3);
					}
				}
			}

			if (_modal)
				InputManager::getInstance().addWidgetModal(mess->mMainWidget);

			return mess;
		}

	/*events:*/
		/** Event : button on message window pressed.\n
			signature : void method(tools::Message* _sender, MessageBoxStyle _result)\n
			@param _sender widget that called this event
			@param _result - id of pressed button
		*/
		EventHandle_MessageBoxPtrMessageStyle
			eventMessageBoxResult;

	protected:
		void updateSize()
		{
			ISubWidgetText* text = nullptr;
			if (mWidgetText != nullptr)
				text = mWidgetText->getSubWidgetText();
			IntSize size = text == nullptr ? IntSize() : text->getTextSize();
			// минимум высота иконки
			if ((nullptr != mIcon) && (mIcon->getImageIndex() != ITEM_NONE))
			{
				if (size.height < mIcon->getHeight())
					size.height = mIcon->getHeight();
				size.width += mIcon->getSize().width;
			}
			size += mOffsetText;
			size.width += 3;

			int width = ((int)mVectorButton.size() * mButtonSize.width) + (((int)mVectorButton.size() + 1) * mButtonOffset.width);
			if (size.width < width)
				size.width = width;

			int offset = (size.width - width) / 2;
			offset += mButtonOffset.width;

			size.width += mMainWidget->getWidth() - mMainWidget->getClientCoord().width;
			size.height += mMainWidget->getHeight() - mMainWidget->getClientCoord().height;

			const IntSize& view = RenderManager::getInstance().getViewSize();
			mMainWidget->setCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height);

			if (nullptr != mIcon)
			{
				if (mWidgetText != nullptr)
				{
					if (mIcon->getImageIndex() != ITEM_NONE)
						mWidgetText->setCoord(mLeftOffset2, mWidgetText->getTop(), mWidgetText->getWidth(), mWidgetText->getHeight());
					else
						mWidgetText->setCoord(mLeftOffset1, mWidgetText->getTop(), mWidgetText->getWidth(), mWidgetText->getHeight());
				}
			}

			for (std::vector<Button*>::iterator iter = mVectorButton.begin(); iter != mVectorButton.end(); ++iter)
			{
				(*iter)->setCoord(offset, mMainWidget->getClientCoord().height - mButtonOffset.height, mButtonSize.width, mButtonSize.height);
				offset += mButtonOffset.width + mButtonSize.width;
			}
		}

		void notifyButtonClick(Widget* _sender)
		{
			_destroyMessage(*_sender->_getInternalData<MessageBoxStyle>());
		}

		void clearButton()
		{
			for (std::vector<Button*>::iterator iter = mVectorButton.begin(); iter != mVectorButton.end(); ++iter)
				WidgetManager::getInstance().destroyWidget(*iter);
			mVectorButton.clear();
		}

		/*void onKeyButtonPressed(KeyCode _key, Char _char)
		{
			Base::onKeyButtonPressed(_key, _char);

			if ((_key == KeyCode::Return) || (_key == KeyCode::NumpadEnter))
				_destroyMessage(mInfoOk);
			else if (_key == KeyCode::Escape)
				_destroyMessage(mInfoCancel);
		}*/

		void _destroyMessage(MessageBoxStyle _result)
		{
			eventMessageBoxResult(this, _result);

			delete this;
		}

		UString getButtonName(MessageBoxStyle _style) const
		{
			size_t index = _style.getButtonIndex();
			const char* tag = getButtonTag(index);
			UString result = LanguageManager::getInstance().replaceTags(utility::toString("#{", tag, "}"));
			if (result == tag)
				return getButtonName(index);
			return result;
		}

		const char* getIconName(size_t _index) const
		{
			static const size_t CountIcons = 4;
			static const char* IconNames[CountIcons + 1] = { "Info", "Quest", "Error", "Warning", "" };
			if (_index >= CountIcons)
				return IconNames[CountIcons];
			return IconNames[_index];
		}

		const char* getButtonName(size_t _index) const
		{
			static const size_t Count = 9;
			static const char * Names[Count + 1] = { "Ok", "Yes", "No", "Abort", "Retry", "Ignore", "Cancel", "Try", "Continue", "" };
			if (_index >= Count)
				return Names[Count];
			return Names[_index];
		}

		const char* getButtonTag(size_t _index) const
		{
			static const size_t Count = 9;
			static const char* Names[Count + 1] = { "MessageBox_Ok", "MessageBox_Yes", "MessageBox_No", "MessageBox_Abort", "MessageBox_Retry", "MessageBox_Ignore", "MessageBox_Cancel", "MessageBox_Try", "MessageBox_Continue", "" };
			if (_index >= Count)
				return Names[Count];
			return Names[_index];
		}

	private:
		void initialise()
		{
			assignWidget(mWidgetText, "Text", false);
			if (mWidgetText != nullptr)
			{
				mOffsetText.set(mMainWidget->getClientCoord().width - mWidgetText->getWidth(), mMainWidget->getClientCoord().height - mWidgetText->getHeight());
				mLeftOffset2 = mLeftOffset1 = mWidgetText->getLeft();
			}

			assignWidget(mIcon, "Icon", false);
			if (mIcon != nullptr)
			{
				mLeftOffset2 = mIcon->getRight() + 3;
			}

			mButtonType = Button::getClassTypeName();

			if (mMainWidget->isUserString("ButtonSkin"))
				mButtonSkin = mMainWidget->getUserString("ButtonSkin");

			Widget* widget = nullptr;
			assignWidget(widget, "ButtonPlace", false);
			if (widget != nullptr)
			{
				mButtonOffset.set(widget->getLeft(), mMainWidget->getClientCoord().height - widget->getTop());
				widget->setVisible(false);
			}

			assignWidget(widget, "ButtonTemplate", false);
			if (widget != nullptr)
			{
				mButtonSize = widget->getSize();
			}

			Window* window = mMainWidget->castType<Window>(false);
			if (window != nullptr)
				window->eventWindowButtonPressed += newDelegate(this, &Message::notifyWindowButtonPressed);
		}

		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
		{
			if (_name == "close")
				endMessage();
		}

	private:
		IntSize mOffsetText;
		TextBox* mWidgetText;

		std::string mButtonSkin;
		std::string mButtonType;
		IntSize mButtonSize;
		IntSize mButtonOffset;

		std::vector<Button*> mVectorButton;
		MessageBoxStyle mInfoOk;
		MessageBoxStyle mInfoCancel;
		bool mSmoothShow;

		std::string mDefaultCaption;
		ImageBox* mIcon;
		int mLeftOffset1;
		int mLeftOffset2;
	};

} // namespace MyGUI

#endif // MESSAGE_BOX_H_
