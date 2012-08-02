/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyInt2Control.h"
#include "Localise.h"

namespace tools
{

	PropertyInt2Control::PropertyInt2Control() :
		mName(nullptr),
		mEdit(nullptr)
	{
	}

	PropertyInt2Control::~PropertyInt2Control()
	{
		mEdit->eventEditTextChange -= MyGUI::newDelegate(this, &PropertyInt2Control::notifyEditTextChange);
	}

	void PropertyInt2Control::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		PropertyControl::OnInitialise(_parent, _place, "PropertyEditControl.layout");

		assignWidget(mName, "Name", false);
		assignWidget(mEdit, "Edit");

		mEdit->eventEditTextChange += MyGUI::newDelegate(this, &PropertyInt2Control::notifyEditTextChange);
	}

	void PropertyInt2Control::updateCaption()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
			mName->setCaption(proper->getType()->getName());
	}

	void PropertyInt2Control::updateProperty()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			mEdit->setEnabled(!proper->getType()->getReadOnly());
			if (mEdit->getOnlyText() != proper->getValue())
				mEdit->setCaption(proper->getValue());

			bool validate = isValidate();
			setColour(validate);
		}
		else
		{
			mEdit->setCaption("");
			mEdit->setEnabled(false);
		}
	}

	void PropertyInt2Control::notifyEditTextChange(MyGUI::EditBox* _sender)
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			bool validate = isValidate();
			if (validate)
				executeAction(getClearValue());

			setColour(validate);
		}
	}

	bool PropertyInt2Control::isValidate()
	{
		MyGUI::UString value = mEdit->getOnlyText();

		int value1 = 0;
		int value2 = 0;
		if (!MyGUI::utility::parseComplex(value, value1, value2))
			return false;

		return true;
	}

	MyGUI::UString PropertyInt2Control::getClearValue()
	{
		MyGUI::UString value = mEdit->getOnlyText();

		int value1 = 0;
		int value2 = 0;
		if (MyGUI::utility::parseComplex(value, value1, value2))
			return MyGUI::utility::toString(value1, " ", value2);

		return "";
	}

	void PropertyInt2Control::setColour(bool _validate)
	{
		MyGUI::UString value = mEdit->getOnlyText();
		if (!_validate)
			value = replaceTags("ColourError") + value;

		size_t index = mEdit->getTextCursor();
		mEdit->setCaption(value);
		mEdit->setTextCursor(index);
	}

}
