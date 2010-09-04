/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "PropertyAlignControl.h"

namespace tools
{

	PropertyAlignControl::PropertyAlignControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("PropertyComboBoxControl.layout", _parent),
		mComboBox(nullptr)
	{
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

		mComboBox->eventComboChangePosition += MyGUI::newDelegate(this, &PropertyAlignControl::notifyComboChangePosition);
	}

	PropertyAlignControl::~PropertyAlignControl()
	{
		mComboBox->eventComboChangePosition -= MyGUI::newDelegate(this, &PropertyAlignControl::notifyComboChangePosition);
	}

	void PropertyAlignControl::updateProperty()
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

	void PropertyAlignControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
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

	size_t PropertyAlignControl::getComboIndex(const MyGUI::UString& _name)
	{
		size_t result = MyGUI::ITEM_NONE;

		size_t count = mComboBox->getItemCount();
		for (size_t index=0; index<count; ++index)
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
