/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "TextFieldControl.h"

namespace tools
{
	TextFieldControl::TextFieldControl() :
		Dialog("TextField.layout"),
		mText(nullptr),
		mOk(nullptr),
		mCancel(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mOk, "Ok");
		assignWidget(mCancel, "Cancel");

		mOk->eventMouseButtonClick += MyGUI::newDelegate(this, &TextFieldControl::notifyOk);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &TextFieldControl::notifyCancel);
		mText->eventEditSelectAccept += MyGUI::newDelegate(this, &TextFieldControl::notifyTextAccept);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &TextFieldControl::notifyWindowButtonPressed);

		mMainWidget->setVisible(false);
	}

	TextFieldControl::~TextFieldControl()
	{
		mOk->eventMouseButtonClick -= MyGUI::newDelegate(this, &TextFieldControl::notifyOk);
		mCancel->eventMouseButtonClick -= MyGUI::newDelegate(this, &TextFieldControl::notifyCancel);
		mText->eventEditSelectAccept -= MyGUI::newDelegate(this, &TextFieldControl::notifyTextAccept);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed -= MyGUI::newDelegate(this, &TextFieldControl::notifyWindowButtonPressed);
	}

	void TextFieldControl::notifyOk(MyGUI::Widget* _sender)
	{
		eventEndDialog(this, true);
	}

	void TextFieldControl::notifyCancel(MyGUI::Widget* _sender)
	{
		eventEndDialog(this, false);
	}

	void TextFieldControl::setCaption(const MyGUI::UString& _value)
	{
		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->setCaption(_value);
	}

	void TextFieldControl::setTextField(const MyGUI::UString& _value)
	{
		mText->setCaption(_value);
	}

	MyGUI::UString TextFieldControl::getTextField()
	{
		return mText->getOnlyText();
	}

	void TextFieldControl::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _buttonName)
	{
		if (_buttonName == "close")
			eventEndDialog(this, false);
	}

	void TextFieldControl::setUserData(MyGUI::Any _data)
	{
		mMainWidget->setUserData(_data);
	}

	void TextFieldControl::notifyTextAccept(MyGUI::Edit* _sender)
	{
		eventEndDialog(this, true);
	}

	void TextFieldControl::onDoModal()
	{
		mText->setTextSelection(0, mText->getTextLength());
		MyGUI::InputManager::getInstance().setKeyFocusWidget(mText);

		MyGUI::IntSize windowSize = mMainWidget->getSize();
		MyGUI::IntSize parentSize = mMainWidget->getParentSize();

		mMainWidget->setPosition((parentSize.width - windowSize.width) / 2, (parentSize.height - windowSize.height) / 2);
	}

	void TextFieldControl::onEndModal()
	{
	}

} // namespace tools
