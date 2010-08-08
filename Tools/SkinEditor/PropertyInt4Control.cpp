/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "PropertyInt4Control.h"

namespace tools
{

	PropertyInt4Control::PropertyInt4Control(MyGUI::Widget* _parent) :
		wraps::BaseLayout("PropertyEditControl.layout", _parent),
		mEdit(nullptr)
	{
		assignWidget(mEdit, "Edit");

	}

	PropertyInt4Control::~PropertyInt4Control()
	{
	}

	void PropertyInt4Control::updateProperty()
	{
		Property* prop = getProperty();
		if (prop != nullptr)
		{
			mEdit->setEnabled(true);
			//size_t index = getComboIndex(prop->getValue());
			//mComboBox->setIndexSelected(index);
		}
		else
		{
			//mComboBox->setIndexSelected(MyGUI::ITEM_NONE);
			mEdit->setEnabled(false);
		}
	}

	/*void PropertyInt4Control::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		Property* prop = getProperty();
		if (prop != nullptr)
		{
			if (_index != MyGUI::ITEM_NONE)
				prop->setValue(mComboBox->getItemNameAt(_index), getTypeName());
			else
				prop->setValue("", getTypeName());
		}
	}*/

} // namespace tools
