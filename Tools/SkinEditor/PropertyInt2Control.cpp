/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "PropertyInt2Control.h"

namespace tools
{

	PropertyInt2Control::PropertyInt2Control(MyGUI::Widget* _parent) :
		wraps::BaseLayout("PropertyEditControl.layout", _parent),
		mEdit(nullptr)
	{
		assignWidget(mEdit, "Edit");

		mEdit->eventEditTextChange += MyGUI::newDelegate(this, &PropertyInt2Control::notifyEditTextChange);
	}

	PropertyInt2Control::~PropertyInt2Control()
	{
		mEdit->eventEditTextChange -= MyGUI::newDelegate(this, &PropertyInt2Control::notifyEditTextChange);
	}

	void PropertyInt2Control::updateProperty()
	{
		Property* proper = getProperty();
		if (proper != nullptr)
		{
			mEdit->setEnabled(true);
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

	void PropertyInt2Control::notifyEditTextChange(MyGUI::Edit* _sender)
	{
		Property* proper = getProperty();
		if (proper != nullptr)
		{
			bool validate = isValidate();
			if (validate)
				proper->setValue(getClearValue(), getTypeName());

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
			value = "#FF0000" + value;

		size_t index = mEdit->getTextCursor();
		mEdit->setCaption(value);
		mEdit->setTextCursor(index);
	}

} // namespace tools
