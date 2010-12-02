/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#include "Precompiled.h"
#include "MessageBox.h"

namespace tools
{
	Message::Message() :
		wraps::BaseLayout("MessageBox.layout"),
		mWidgetText(nullptr),
		mInfoOk(MessageBoxStyle::None),
		mInfoCancel(MessageBoxStyle::None),
		mSmoothShow(false),
		mIcon(nullptr),
		mLeftOffset1(0),
		mLeftOffset2(0)
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

		mButtonType = MyGUI::Button::getClassTypeName();

		if (mMainWidget->isUserString("ButtonSkin"))
			mButtonSkin = mMainWidget->getUserString("ButtonSkin");

		MyGUI::Widget* widget = nullptr;
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
	}

	Message::~Message()
	{
		mWidgetText = nullptr;
		mIcon = nullptr;
	}

	void Message::setMessageText(const MyGUI::UString& _message)
	{
		if (mWidgetText != nullptr)
			mWidgetText->setCaption(_message);
		updateSize();
	}

	MessageBoxStyle Message::addButtonName(const MyGUI::UString& _name)
	{
		//FIXME
		if (mVectorButton.size() >= MessageBoxStyle::_CountUserButtons)
		{
			MYGUI_LOG(Warning, "Too many buttons in message box, ignored");
			return MessageBoxStyle::None;
		}
		// бит, номер кнопки + смещение до Button1
		MessageBoxStyle info = MessageBoxStyle(MessageBoxStyle::Enum(MYGUI_FLAG(mVectorButton.size() + MessageBoxStyle::_IndexUserButton1)));

		// запоминаем кнопки дл€ отмены и подтверждени€
		if (mVectorButton.empty())
			mInfoOk = info;
		mInfoCancel = info;

		MyGUI::Widget* widget = mMainWidget->createWidgetT(mButtonType, mButtonSkin, MyGUI::IntCoord(), MyGUI::Align::Left | MyGUI::Align::Bottom);
		MyGUI::Button* button = widget->castType<MyGUI::Button>();
		button->eventMouseButtonClick += newDelegate(this, &Message::notifyButtonClick);
		button->setCaption(_name);
		button->_setInternalData(info);
		mVectorButton.push_back(button);

		updateSize();
		return info;
	}

	void Message::setMessageIcon(MessageBoxStyle _icon)
	{
		if (nullptr == mIcon)
			return;

		if (mIcon->getItemResource() != nullptr)
		{
			mIcon->setItemName(getIconName(_icon.getIconIndex()));
		}
		else
		{
			mIcon->setImageIndex(_icon.getIconIndex());
		}

		updateSize();
	}

	void Message::setMessageButton(MessageBoxStyle _info)
	{
		clearButton();

		std::vector<MessageBoxStyle> buttons = _info.getButtons();

		for (size_t index = 0; index < buttons.size(); ++index)
		{
			// корректируем ее номер
			MessageBoxStyle info = buttons[index];

			// если бит есть то ставим кнопку
			addButtonName(getButtonName(info));

			// внутри адд сбрасываетс€
			mVectorButton.back()->_setInternalData(info);

			// перва€ кнопка
			if (mVectorButton.size() == 1)
				mInfoOk = info;
			// последн€€ кнопка
			mInfoCancel = info;
		}

		updateSize();
	}

	void Message::setMessageStyle(MessageBoxStyle _style)
	{
		setMessageButton(_style);
		setMessageIcon(_style);
	}

	void Message::notifyButtonClick(MyGUI::Widget* _sender)
	{
		_destroyMessage(*_sender->_getInternalData<MessageBoxStyle>());
	}

	void Message::clearButton()
	{
		for (std::vector<MyGUI::Button*>::iterator iter = mVectorButton.begin(); iter != mVectorButton.end(); ++iter)
			MyGUI::WidgetManager::getInstance().destroyWidget(*iter);
		mVectorButton.clear();
	}

	/*void Message::onKeyButtonPressed(MyGUI::KeyCode _key, MyGUI::Char _char)
	{
		Base::onKeyButtonPressed(_key, _char);

		if ((_key == KeyCode::Return) || (_key == KeyCode::NumpadEnter))
			_destroyMessage(mInfoOk);
		else if (_key == KeyCode::Escape)
			_destroyMessage(mInfoCancel);
	}*/

	void Message::_destroyMessage(MessageBoxStyle _result)
	{
		eventMessageBoxResult(this, _result);

		delete this;
		/*if (mSmoothShow)
			mMainWidget->destroySmooth();
		else
			MyGUI::WidgetManager::getInstance().destroyWidget(this);*/
	}

	void Message::setSmoothShow(bool _smooth)
	{
		mSmoothShow = _smooth;
		if (mSmoothShow)
		{
			mMainWidget->setAlpha(MyGUI::ALPHA_MIN);
			mMainWidget->setVisible(true);
			mMainWidget->castType<MyGUI::Window>()->setVisibleSmooth(true);
		}
	}

	const char* Message::getIconName(size_t _index)
	{
		static const size_t CountIcons = 4;
		static const char* IconNames[CountIcons + 1] = { "Info", "Quest", "Error", "Warning", "" };
		if (_index >= CountIcons)
			return IconNames[CountIcons];
		return IconNames[_index];
	}

	Message* Message::createMessageBox(
		const MyGUI::UString& _caption,
		const MyGUI::UString& _message,
		MessageBoxStyle _style,
		const std::string& _layer,
		bool _modal,
		const std::string& _button1,
		const std::string& _button2,
		const std::string& _button3,
		const std::string& _button4)
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
			MyGUI::InputManager::getInstance().addWidgetModal(mess->mMainWidget);

		return mess;
	}

	void Message::updateSize()
	{
		MyGUI::ISubWidgetText* text = nullptr;
		if (mWidgetText != nullptr)
			text = mWidgetText->getSubWidgetText();
		MyGUI::IntSize size = text == nullptr ? MyGUI::IntSize() : text->getTextSize();
		// минимум высота иконки
		if ((nullptr != mIcon) && (mIcon->getImageIndex() != MyGUI::ITEM_NONE))
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

		const MyGUI::IntSize& view = MyGUI::RenderManager::getInstance().getViewSize();
		mMainWidget->setCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height);

		if (nullptr != mIcon)
		{
			if (mWidgetText != nullptr)
			{
				if (mIcon->getImageIndex() != MyGUI::ITEM_NONE)
					mWidgetText->setCoord(mLeftOffset2, mWidgetText->getTop(), mWidgetText->getWidth(), mWidgetText->getHeight());
				else
					mWidgetText->setCoord(mLeftOffset1, mWidgetText->getTop(), mWidgetText->getWidth(), mWidgetText->getHeight());
			}
		}

		for (std::vector<MyGUI::Button*>::iterator iter = mVectorButton.begin(); iter != mVectorButton.end(); ++iter)
		{
			(*iter)->setCoord(offset, mMainWidget->getClientCoord().height - mButtonOffset.height, mButtonSize.width, mButtonSize.height);
			offset += mButtonOffset.width + mButtonSize.width;
		}
	}

	void Message::setMessageModal(bool _value)
	{
		if (_value)
			MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
		else
			MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
	}

	MyGUI::UString Message::getButtonName(MessageBoxStyle _style)
	{
		size_t index = _style.getButtonIndex();
		const char* tag = getButtonTag(index);
		MyGUI::UString result = MyGUI::LanguageManager::getInstance().replaceTags(MyGUI::utility::toString("#{", tag, "}"));
		if (result == tag)
			return getButtonName(index);
		return result;
	}

	const char* Message::getButtonName(size_t _index)
	{
		static const size_t Count = 9;
		static const char* Names[Count + 1] = { "Ok", "Yes", "No", "Abort", "Retry", "Ignore", "Cancel", "Try", "Continue", "" };
		if (_index >= Count)
			return Names[Count];
		return Names[_index];
	}

	const char* Message::getButtonTag(size_t _index)
	{
		static const size_t Count = 9;
		static const char* Names[Count + 1] = { "MessageBox_Ok", "MessageBox_Yes", "MessageBox_No", "MessageBox_Abort", "MessageBox_Retry", "MessageBox_Ignore", "MessageBox_Cancel", "MessageBox_Try", "MessageBox_Continue", "" };
		if (_index >= Count)
			return Names[Count];
		return Names[_index];
	}

	void Message::endMessage(MessageBoxStyle _result)
	{
		_destroyMessage(_result);
	}

	void Message::endMessage()
	{
		_destroyMessage(mInfoCancel);
	}

	void Message::setCaption(const MyGUI::UString& _value)
	{
		mMainWidget->castType<MyGUI::Window>()->setCaption(_value);
	}

} // namespace tools
