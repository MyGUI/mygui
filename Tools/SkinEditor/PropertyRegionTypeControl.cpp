/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyRegionTypeControl.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(PropertyRegionTypeControl)

	PropertyRegionTypeControl::PropertyRegionTypeControl() :
		mName(nullptr),
		mComboBox(nullptr)
	{
	}

	PropertyRegionTypeControl::~PropertyRegionTypeControl()
	{
		mComboBox->eventComboChangePosition -= MyGUI::newDelegate(this, &PropertyRegionTypeControl::notifyComboChangePosition);
	}

	void PropertyRegionTypeControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		PropertyControl::OnInitialise(_parent, _place, "PropertyComboBoxControl.layout");

		assignWidget(mName, "Name", false);
		assignWidget(mComboBox, "ComboBox");

		mComboBox->addItem("SubSkin");
		mComboBox->addItem("TileRect");
		mComboBox->addItem("TileRect Horz");
		mComboBox->addItem("TileRect Vert");

		mComboBox->beginToItemFirst();

		mComboBox->eventComboChangePosition += MyGUI::newDelegate(this, &PropertyRegionTypeControl::notifyComboChangePosition);
	}

	void PropertyRegionTypeControl::updateCaption()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
			mName->setCaption(proper->getType()->getName());
	}

	void PropertyRegionTypeControl::updateProperty()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			mComboBox->setEnabled(!proper->getType()->getReadOnly());
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
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			std::string value = _index != MyGUI::ITEM_NONE ? mComboBox->getItemNameAt(_index) : "";
			executeAction(value);
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

}
