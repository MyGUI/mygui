/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyBoolControl.h"

namespace tools
{

	PropertyBoolControl::~PropertyBoolControl()
	{
		mComboBox->eventComboChangePosition -=
			MyGUI::newDelegate(this, &PropertyBoolControl::notifyComboChangePosition);
	}

	void PropertyBoolControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view /*_layoutName*/)
	{
		PropertyControl::OnInitialise(_parent, _place, "PropertyComboBoxControl.layout");

		assignWidget(mName, "Name", false);
		assignWidget(mComboBox, "ComboBox");

		mComboBox->addItem("true");
		mComboBox->addItem("false");

		mComboBox->beginToItemFirst();

		mComboBox->eventComboChangePosition +=
			MyGUI::newDelegate(this, &PropertyBoolControl::notifyComboChangePosition);
	}

	void PropertyBoolControl::updateCaption()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
			mName->setCaption(proper->getType()->getName());
	}

	void PropertyBoolControl::updateProperty()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			size_t index = getComboIndex(proper->getValue());
			mComboBox->setIndexSelected(index);
		}
		else
		{
			mComboBox->setIndexSelected(MyGUI::ITEM_NONE);
		}
	}

	void PropertyBoolControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			std::string_view value;
			if (_index != MyGUI::ITEM_NONE)
				value = mComboBox->getItemNameAt(_index);
			executeAction(value);
		}
	}

	size_t PropertyBoolControl::getComboIndex(const MyGUI::UString& _name)
	{
		size_t result = MyGUI::ITEM_NONE;

		size_t count = mComboBox->getItemCount();
		for (size_t index = 0; index < count; ++index)
		{
			if (mComboBox->getItemNameAt(index) == _name)
			{
				result = index;
				break;
			}
		}

		return result;
	}

}
