/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyAlignControl.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(PropertyAlignControl)

	PropertyAlignControl::PropertyAlignControl() :
		mName(nullptr),
		mComboBox(nullptr)
	{
	}

	PropertyAlignControl::~PropertyAlignControl()
	{
		mComboBox->eventComboChangePosition -= MyGUI::newDelegate(this, &PropertyAlignControl::notifyComboChangePosition);
	}

	void PropertyAlignControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		PropertyControl::OnInitialise(_parent, _place, "PropertyComboBoxControl.layout");

		assignWidget(mName, "Name", false);
		assignWidget(mComboBox, "ComboBox");

		mComboBox->addItem("Default");
		mComboBox->addItem("Stretch");
		mComboBox->addItem("Center");

		mComboBox->addItem("Left Top");
		mComboBox->addItem("Left Bottom");
		mComboBox->addItem("Left VStretch");
		mComboBox->addItem("Left VCenter");

		mComboBox->addItem("Right Top");
		mComboBox->addItem("Right Bottom");
		mComboBox->addItem("Right VStretch");
		mComboBox->addItem("Right VCenter");

		mComboBox->addItem("HStretch Top");
		mComboBox->addItem("HStretch Bottom");
		mComboBox->addItem("HStretch VStretch");
		mComboBox->addItem("HStretch VCenter");

		mComboBox->addItem("HCenter Bottom");
		mComboBox->addItem("HCenter VStretch");
		mComboBox->addItem("HCenter VCenter");
		mComboBox->beginToItemFirst();

		mComboBox->eventComboChangePosition += MyGUI::newDelegate(this, &PropertyAlignControl::notifyComboChangePosition);
	}

	void PropertyAlignControl::updateCaption()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
			mName->setCaption(proper->getType()->getName());
	}

	void PropertyAlignControl::updateProperty()
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

	void PropertyAlignControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			std::string value = _index != MyGUI::ITEM_NONE ? mComboBox->getItemNameAt(_index) : "";
			executeAction(value);
		}
	}

	size_t PropertyAlignControl::getComboIndex(const MyGUI::UString& _name)
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
