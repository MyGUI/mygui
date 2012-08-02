/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyStringControl.h"
#include "Localise.h"

namespace tools
{

	PropertyStringControl::PropertyStringControl() :
		mName(nullptr),
		mEdit(nullptr)
	{
	}

	PropertyStringControl::~PropertyStringControl()
	{
		mEdit->eventEditTextChange -= MyGUI::newDelegate(this, &PropertyStringControl::notifyEditTextChange);
	}

	void PropertyStringControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		PropertyControl::OnInitialise(_parent, _place, "PropertyEditControl.layout");

		assignWidget(mName, "Name", false);
		assignWidget(mEdit, "Edit");

		mEdit->eventEditTextChange += MyGUI::newDelegate(this, &PropertyStringControl::notifyEditTextChange);
	}

	void PropertyStringControl::updateCaption()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
			mName->setCaption(proper->getType()->getName());
	}

	void PropertyStringControl::updateProperty()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			mEdit->setEnabled(!proper->getType()->getReadOnly());
			if (mEdit->getOnlyText() != proper->getValue())
				mEdit->setCaption(proper->getValue());
		}
		else
		{
			mEdit->setCaption("");
			mEdit->setEnabled(false);
		}
	}

	void PropertyStringControl::notifyEditTextChange(MyGUI::EditBox* _sender)
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
			executeAction(getClearValue());
	}

	MyGUI::UString PropertyStringControl::getClearValue()
	{
		MyGUI::UString value = mEdit->getOnlyText();
		return value;
	}

}
