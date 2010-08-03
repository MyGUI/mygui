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
		wraps::BaseLayout("TextFieldControl.layout"),
		mText(nullptr),
		mOk(nullptr),
		mCancel(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mOk, "Ok");
		assignWidget(mCancel, "Cancel");

		mOk->eventMouseButtonClick += MyGUI::newDelegate(this, &TextFieldControl::notifyOk);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &TextFieldControl::notifyCancel);
	}

	TextFieldControl::~TextFieldControl()
	{
		mOk->eventMouseButtonClick -= MyGUI::newDelegate(this, &TextFieldControl::notifyOk);
		mCancel->eventMouseButtonClick -= MyGUI::newDelegate(this, &TextFieldControl::notifyCancel);
	}

	void TextFieldControl::notifyOk(MyGUI::Widget* _sender)
	{
	}

	void TextFieldControl::notifyCancel(MyGUI::Widget* _sender)
	{
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

	const MyGUI::UString& TextFieldControl::getTextField()
	{
		return mText->getCaption();
	}

} // namespace tools
