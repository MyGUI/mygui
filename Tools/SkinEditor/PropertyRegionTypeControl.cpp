/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "PropertyRegionTypeControl.h"

namespace tools
{

	PropertyRegionTypeControl::PropertyRegionTypeControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("PropertyComboBoxControl.layout", _parent),
		mComboBox(nullptr)
	{
		assignWidget(mComboBox, "ComboBox");

		mComboBox->addItem("EditText");
		mComboBox->addItem("SimpleText");

		mComboBox->addItem("SubSkin");

		mComboBox->addItem("TileRect");
		mComboBox->addItem("TileRect Hor");
		mComboBox->addItem("TileRect Ver");

		mComboBox->eventComboChangePosition += MyGUI::newDelegate(this, &PropertyRegionTypeControl::notifyComboChangePosition);
	}

	PropertyRegionTypeControl::~PropertyRegionTypeControl()
	{
		mComboBox->eventComboChangePosition -= MyGUI::newDelegate(this, &PropertyRegionTypeControl::notifyComboChangePosition);
	}

	void PropertyRegionTypeControl::updateProperty()
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

	void PropertyRegionTypeControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
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

	size_t PropertyRegionTypeControl::getComboIndex(const MyGUI::UString& _name)
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
