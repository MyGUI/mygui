/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "PropertyInt4Control.h"

namespace tools
{

	PropertyInt4Control::PropertyInt4Control(MyGUI::Widget* _parent) :
		wraps::BaseLayout("PropertyEditControl.layout", _parent),
		mEdit(nullptr)
	{
		assignWidget(mEdit, "Edit");

		mEdit->eventEditTextChange += MyGUI::newDelegate(this, &PropertyInt4Control::notifyEditTextChange);
	}

	PropertyInt4Control::~PropertyInt4Control()
	{
		mEdit->eventEditTextChange -= MyGUI::newDelegate(this, &PropertyInt4Control::notifyEditTextChange);
	}

	void PropertyInt4Control::updateProperty()
	{
		Property* prop = getProperty();
		if (prop != nullptr)
		{
			mEdit->setEnabled(true);
			mEdit->setCaption(prop->getValue());

			bool validate = isValidate();
			setColour(validate);
		}
		else
		{
			mEdit->setCaption("");
			mEdit->setEnabled(false);
		}
	}

	void PropertyInt4Control::notifyEditTextChange(MyGUI::Edit* _sender)
	{
		Property* prop = getProperty();
		if (prop != nullptr)
		{
			bool validate = isValidate();
			if (validate)
				prop->setValue(getClearValue(), getTypeName());

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
			value = "#FF0000" + value;

		size_t index = mEdit->getTextCursor();
		mEdit->setCaption(value);
		mEdit->setTextCursor(index);
	}

} // namespace tools
