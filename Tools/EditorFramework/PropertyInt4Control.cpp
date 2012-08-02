/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyInt4Control.h"
#include "Localise.h"

namespace tools
{

	PropertyInt4Control::PropertyInt4Control() :
		mName(nullptr),
		mEdit(nullptr)
	{
	}

	PropertyInt4Control::~PropertyInt4Control()
	{
		mEdit->eventEditTextChange -= MyGUI::newDelegate(this, &PropertyInt4Control::notifyEditTextChange);
	}

	void PropertyInt4Control::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		PropertyControl::OnInitialise(_parent, _place, "PropertyEditControl.layout");

		assignWidget(mName, "Name", false);
		assignWidget(mEdit, "Edit");

		mEdit->eventEditTextChange += MyGUI::newDelegate(this, &PropertyInt4Control::notifyEditTextChange);
	}

	void PropertyInt4Control::updateCaption()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
			mName->setCaption(proper->getType()->getName());
	}

	void PropertyInt4Control::updateProperty()
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

	void PropertyInt4Control::notifyEditTextChange(MyGUI::EditBox* _sender)
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

	bool PropertyInt4Control::isValidate()
	{
		MyGUI::UString value = mEdit->getOnlyText();

		int value1 = 0;
		int value2 = 0;
		int value3 = 0;
		int value4 = 0;
		if (!MyGUI::utility::parseComplex(value, value1, value2, value3, value4))
			return false;

		return true;
	}

	MyGUI::UString PropertyInt4Control::getClearValue()
	{
		MyGUI::UString value = mEdit->getOnlyText();

		int value1 = 0;
		int value2 = 0;
		int value3 = 0;
		int value4 = 0;
		if (MyGUI::utility::parseComplex(value, value1, value2, value3, value4))
			return MyGUI::utility::toString(value1, " ", value2, " ", value3, " ", value4);

		return "";
	}

	void PropertyInt4Control::setColour(bool _validate)
	{
		MyGUI::UString value = mEdit->getOnlyText();
		if (!_validate)
			value = replaceTags("ColourError") + value;

		size_t index = mEdit->getTextCursor();
		mEdit->setCaption(value);
		mEdit->setTextCursor(index);
	}

}
