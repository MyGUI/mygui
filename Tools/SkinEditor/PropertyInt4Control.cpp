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
		Property* proper = getProperty();
		if (proper != nullptr)
		{
			mEdit->setEnabled(!proper->getReadOnly());
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

	void PropertyInt4Control::notifyEditTextChange(MyGUI::Edit* _sender)
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

} // namespace tools
