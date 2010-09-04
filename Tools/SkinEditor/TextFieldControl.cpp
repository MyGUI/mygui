/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "TextFieldControl.h"

namespace tools
{

	TextFieldControl::TextFieldControl() :
		wraps::BaseLayout("TextField.layout"),
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
		eventResult(true);
	}

	void TextFieldControl::notifyCancel(MyGUI::Widget* _sender)
	{
		eventResult(false);
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
			eventResult(false);
	}

	void TextFieldControl::show()
	{
		mMainWidget->setVisible(true);
		MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);

		mText->setTextSelection(0, mText->getTextLength());

		MyGUI::InputManager::getInstance().setKeyFocusWidget(mText);
	}

	void TextFieldControl::hide()
	{
		MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
		mMainWidget->setVisible(false);
	}

	void TextFieldControl::setUserData(MyGUI::Any _data)
	{
		mMainWidget->setUserData(_data);
	}

	void TextFieldControl::notifyTextAccept(MyGUI::Edit* _sender)
	{
		eventResult(true);
	}

} // namespace tools
