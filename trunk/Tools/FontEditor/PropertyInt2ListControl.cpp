/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyInt2ListControl.h"
#include "Localise.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(PropertyInt2ListControl)

	PropertyInt2ListControl::PropertyInt2ListControl() :
		mName(nullptr),
		mList(nullptr)
	{
	}

	PropertyInt2ListControl::~PropertyInt2ListControl()
	{
		//mList->eventEditTextChange -= MyGUI::newDelegate(this, &PropertyInt2ListControl::notifyEditTextChange);
	}

	void PropertyInt2ListControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		PropertyControl::OnInitialise(_parent, _place, "PropertyListControl.layout");

		assignWidget(mName, "Name", false);
		assignWidget(mList, "List");

		//mList->eventEditTextChange += MyGUI::newDelegate(this, &PropertyInt2ListControl::notifyEditTextChange);
	}

	void PropertyInt2ListControl::updateCaption()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
			mName->setCaption(proper->getType()->getName());
	}

	void PropertyInt2ListControl::updateProperty()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			mList->setEnabled(!proper->getType()->getReadOnly());
			std::string value = getValue();
			if (value != proper->getValue())
				setValue(proper->getValue());

			//bool validate = isValidate();
			//setColour(validate);
		}
		else
		{
			mList->removeAllItems();
			mList->setEnabled(false);
		}
	}

	/*void PropertyInt2ListControl::notifyEditTextChange(MyGUI::EditBox* _sender)
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			bool validate = isValidate();
			if (validate)
				executeAction(getClearValue());

			setColour(validate);
		}
	}*/

	/*bool PropertyInt2ListControl::isValidate()
	{
		MyGUI::UString value = getValue();

		int value1 = 0;
		int value2 = 0;
		if (!MyGUI::utility::parseComplex(value, value1, value2))
			return false;

		return true;
	}

	MyGUI::UString PropertyInt2ListControl::getClearValue()
	{
		MyGUI::UString value = mList->getValue();

		int value1 = 0;
		int value2 = 0;
		if (MyGUI::utility::parseComplex(value, value1, value2))
			return MyGUI::utility::toString(value1, " ", value2);

		return "";
	}

	void PropertyInt2ListControl::setColour(bool _validate)
	{
		MyGUI::UString value = mList->getOnlyText();
		if (!_validate)
			value = replaceTags("ColourError") + value;

		size_t index = mList->getTextCursor();
		mList->setCaption(value);
		mList->setTextCursor(index);
		ץו ץו =)
	}*/

	std::string PropertyInt2ListControl::getValue()
	{
		std::string result;

		for (size_t index = 0; index < mList->getItemCount(); index ++)
		{
			if (!result.empty())
				result += "|";
			result += mList->getItemNameAt(index);
		}

		return result;
	}

	void PropertyInt2ListControl::setValue(const std::string& _value)
	{
		mList->removeAllItems();
		std::vector<std::string> values = MyGUI::utility::split(_value, "|");
		for (size_t index = 0; index < values.size(); index ++)
			mList->addItem(values[index]);
	}

}
