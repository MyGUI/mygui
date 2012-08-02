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
		mText(nullptr),
		mOk(nullptr),
		mCancel(nullptr)
	{
	}

	TextFieldControl::~TextFieldControl()
	{
		mMainWidget->eventRootKeyChangeFocus -= MyGUI::newDelegate(this, &TextFieldControl::notifyRootKeyChangeFocus);

		mOk->eventMouseButtonClick -= MyGUI::newDelegate(this, &TextFieldControl::notifyOk);
		mCancel->eventMouseButtonClick -= MyGUI::newDelegate(this, &TextFieldControl::notifyCancel);
		mText->eventEditSelectAccept -= MyGUI::newDelegate(this, &TextFieldControl::notifyTextAccept);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed -= MyGUI::newDelegate(this, &TextFieldControl::notifyWindowButtonPressed);
	}

	void TextFieldControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, "TextField.layout");

		setDialogRoot(mMainWidget);

		assignWidget(mText, "Text");
		assignWidget(mOk, "Ok", false);
		assignWidget(mCancel, "Cancel", false);

		mOk->eventMouseButtonClick += MyGUI::newDelegate(this, &TextFieldControl::notifyOk);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &TextFieldControl::notifyCancel);
		mText->eventEditSelectAccept += MyGUI::newDelegate(this, &TextFieldControl::notifyTextAccept);
		
		mMainWidget->eventRootKeyChangeFocus += MyGUI::newDelegate(this, &TextFieldControl::notifyRootKeyChangeFocus);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &TextFieldControl::notifyWindowButtonPressed);

		mMainWidget->setVisible(false);
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

	void TextFieldControl::notifyTextAccept(MyGUI::EditBox* _sender)
	{
		eventEndDialog(this, true);
	}

	void TextFieldControl::onDoModal()
	{
		mText->setTextSelection(0, mText->getTextLength());
		MyGUI::InputManager::getInstance().setKeyFocusWidget(mText);
	}

	void TextFieldControl::onEndModal()
	{
	}

	void TextFieldControl::notifyRootKeyChangeFocus(MyGUI::Widget* _sender, bool _focus)
	{
		if (!_focus && mMainWidget->getVisible())
			eventEndDialog(this, false);
	}

	void TextFieldControl::setCoord(const MyGUI::IntCoord& _value)
	{
		mMainWidget->setCoord(_value);
	}
}
