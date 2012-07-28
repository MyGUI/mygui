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

		const char* const fileTypesData[] = { "*.ttf", "*.ttc", "*.otf", "*.pfa", "*.pfb", "*.fon", "*.fnt" };

		const MyGUI::VectorString fileTypes(fileTypesData, fileTypesData + sizeof fileTypesData / sizeof *fileTypesData);

		struct StrCmpI : public std::binary_function<std::string, std::string, bool>
		{
			result_type operator()(const first_argument_type& _a, const second_argument_type& _b)
			{
				size_t aLength = _a.length(), bLength = _b.length(), length = (std::min)(aLength, bLength);
				first_argument_type::const_iterator aIter = _a.begin();
				second_argument_type::const_iterator bIter = _b.begin();

				while (length-- > 0)
				{
					int aUpper = toupper(*aIter++), bUpper = toupper(*bIter++);

					if (aUpper != bUpper)
						return aUpper < bUpper;
				}

				return aLength < bLength;
			}
		};

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

		std::set<std::string, StrCmpI> allFilenames;

		for (MyGUI::VectorString::const_iterator fileType = fileTypes.begin(); fileType != fileTypes.end(); ++fileType)
		{
			const MyGUI::VectorString& filenames = MyGUI::DataManager::getInstance().getDataListNames(*fileType);
			allFilenames.insert(filenames.begin(), filenames.end());
		}

		for (std::set<std::string, StrCmpI>::const_iterator iter = allFilenames.begin(); iter != allFilenames.end(); ++iter)
			mResources.push_back(*iter);
	}

}
