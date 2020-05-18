/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyFontSourceControl.h"
#include "FactoryManager.h"

namespace tools
{
	namespace
	{
		const MyGUI::VectorString fileTypes{ "*.ttf", "*.ttc", "*.otf", "*.pfa", "*.pfb", "*.fon", "*.fnt" };
	}

	FACTORY_ITEM_ATTRIBUTE(PropertyFontSourceControl)

	PropertyFontSourceControl::PropertyFontSourceControl() :
		mName(nullptr),
		mComboBox(nullptr)
	{
	}

	PropertyFontSourceControl::~PropertyFontSourceControl()
	{
		mComboBox->eventComboChangePosition -= MyGUI::newDelegate(this, &PropertyFontSourceControl::notifyComboChangePosition);
	}

	void PropertyFontSourceControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		PropertyControl::OnInitialise(_parent, _place, "PropertyComboBoxControl.layout");

		assignWidget(mName, "Name", false);
		assignWidget(mComboBox, "ComboBox");

		fillResources();

		for (MyGUI::VectorString::const_iterator item = mResources.begin(); item != mResources.end(); ++item)
			mComboBox->addItem((*item));

		mComboBox->beginToItemFirst();

		mComboBox->eventComboChangePosition += MyGUI::newDelegate(this, &PropertyFontSourceControl::notifyComboChangePosition);
	}

	void PropertyFontSourceControl::updateCaption()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
			mName->setCaption(proper->getType()->getName());
	}

	void PropertyFontSourceControl::updateProperty()
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

	void PropertyFontSourceControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			std::string value = _index != MyGUI::ITEM_NONE ? mComboBox->getItemNameAt(_index) : "";
			executeAction(value);
		}
	}

	size_t PropertyFontSourceControl::getComboIndex(const MyGUI::UString& _name)
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

	void PropertyFontSourceControl::fillResources()
	{
		mResources.clear();

		std::set<std::string> allFilenames;

		for (const auto& fileType : fileTypes)
		{
			const MyGUI::VectorString& filenames = MyGUI::DataManager::getInstance().getDataListNames(fileType);
			allFilenames.insert(filenames.begin(), filenames.end());
		}

		for (const auto& fileName : allFilenames)
			mResources.push_back(fileName);
	}

}
