/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "PropertyBoolControl.h"

namespace tools
{

	PropertyBoolControl::PropertyBoolControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("PropertyComboBoxControl.layout", _parent),
		mComboBox(nullptr)
	{
		assignWidget(mComboBox, "ComboBox");

		mComboBox->addItem("True");
		mComboBox->addItem("False");

		mComboBox->beginToItemFirst();

		mComboBox->eventComboChangePosition += MyGUI::newDelegate(this, &PropertyBoolControl::notifyComboChangePosition);
	}

	PropertyBoolControl::~PropertyBoolControl()
	{
		mComboBox->eventComboChangePosition -= MyGUI::newDelegate(this, &PropertyBoolControl::notifyComboChangePosition);
	}

	void PropertyBoolControl::updateProperty()
	{
		Property* proper = getProperty();
		if (proper != nullptr)
		{
			mComboBox->setEnabled(!proper->getReadOnly());
			size_t index = getComboIndex(proper->getValue());
			mComboBox->setIndexSelected(index);
		}
		else
		{
			mComboBox->setIndexSelected(MyGUI::ITEM_NONE);
			mComboBox->setEnabled(false);
		}
	}

	void PropertyBoolControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		Property* proper = getProperty();
		if (proper != nullptr)
		{
			if (_index != MyGUI::ITEM_NONE)
				proper->setValue(mComboBox->getItemNameAt(_index), getTypeName());
			else
				proper->setValue("", getTypeName());
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

} // namespace tools
