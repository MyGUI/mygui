/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyIntControl.h"
#include "Localise.h"

namespace tools
{

	PropertyIntControl::PropertyIntControl() :
		mName(nullptr),
		mEdit(nullptr)
	{
	}

	PropertyIntControl::~PropertyIntControl()
	{
		mEdit->eventEditTextChange -= MyGUI::newDelegate(this, &PropertyIntControl::notifyEditTextChange);
	}

	void PropertyIntControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		PropertyControl::OnInitialise(_parent, _place, "PropertyEditControl.layout");

		assignWidget(mName, "Name", false);
		assignWidget(mEdit, "Edit");

		mEdit->eventEditTextChange += MyGUI::newDelegate(this, &PropertyIntControl::notifyEditTextChange);
	}

	void PropertyIntControl::updateCaption()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
			mName->setCaption(proper->getType()->getName());
	}

	void PropertyIntControl::updateProperty()
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

	void PropertyIntControl::notifyEditTextChange(MyGUI::EditBox* _sender)
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

	bool PropertyIntControl::isValidate()
	{
		MyGUI::UString value = mEdit->getOnlyText();

		int value1 = 0;
		if (!MyGUI::utility::parseComplex(value, value1))
			return false;

		return true;
	}

	MyGUI::UString PropertyIntControl::getClearValue()
	{
		MyGUI::UString value = mEdit->getOnlyText();

		int value1 = 0;
		if (MyGUI::utility::parseComplex(value, value1))
			return MyGUI::utility::toString(value1);

		return "";
	}

	void PropertyIntControl::setColour(bool _validate)
	{
		MyGUI::UString value = mEdit->getOnlyText();
		if (!_validate)
			value = replaceTags("ColourError") + value;

		size_t index = mEdit->getTextCursor();
		mEdit->setCaption(value);
		mEdit->setTextCursor(index);
	}

}
