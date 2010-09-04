/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "PropertyTexturesControl.h"

namespace tools
{

	PropertyTexturesControl::PropertyTexturesControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("PropertyComboBoxControl.layout", _parent),
		mComboBox(nullptr)
	{
		assignWidget(mComboBox, "ComboBox");

		// FIXME потом вынести в загружаемые настройки
		MyGUI::VectorString paths = MyGUI::DataManager::getInstance().getDataListNames("*.png");
		for (MyGUI::VectorString::iterator iter=paths.begin(); iter!=paths.end(); ++iter)
			mComboBox->addItem(*iter);

		// FIXME потом вынести в загружаемые настройки
		paths = MyGUI::DataManager::getInstance().getDataListNames("*.jpg");
		for (MyGUI::VectorString::iterator iter=paths.begin(); iter!=paths.end(); ++iter)
			mComboBox->addItem(*iter);

		mComboBox->eventComboChangePosition += MyGUI::newDelegate(this, &PropertyTexturesControl::notifyComboChangePosition);
	}

	PropertyTexturesControl::~PropertyTexturesControl()
	{
		mComboBox->eventComboChangePosition -= MyGUI::newDelegate(this, &PropertyTexturesControl::notifyComboChangePosition);
	}

	void PropertyTexturesControl::updateProperty()
	{
		Property* proper = getProperty();
		if (proper != nullptr)
		{
			mComboBox->setEnabled(true);
			size_t index = getComboIndex(proper->getValue());
			mComboBox->setIndexSelected(index);
		}
		else
		{
			mComboBox->setIndexSelected(MyGUI::ITEM_NONE);
			mComboBox->setEnabled(false);
		}
	}

	void PropertyTexturesControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
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

	size_t PropertyTexturesControl::getComboIndex(const MyGUI::UString& _name)
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
