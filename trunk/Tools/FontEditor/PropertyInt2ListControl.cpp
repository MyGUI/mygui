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
	}

	void PropertyInt2ListControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		PropertyControl::OnInitialise(_parent, _place, "PropertyListControl.layout");

		assignWidget(mName, "Name", false);
		assignWidget(mList, "List");
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
		}
		else
		{
			mList->removeAllItems();
			mList->setEnabled(false);
		}
	}

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
