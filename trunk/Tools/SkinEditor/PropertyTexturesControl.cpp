/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "PropertyTexturesControl.h"

namespace tools
{
	PropertyTexturesControl::PropertyTexturesControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("PropertyTextureBrowseControl.layout", _parent),
		mComboBox(nullptr),
		mBrowse(nullptr),
		mTextureBrowseControl(nullptr)
	{
		assignWidget(mComboBox, "ComboBox");
		assignWidget(mBrowse, "Browse");

		fillTextures();

		for (MyGUI::VectorString::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
			mComboBox->addItem((*item));

		mComboBox->beginToItemFirst();

		mTextureBrowseControl = new TextureBrowseControl();
		mTextureBrowseControl->eventEndDialog = MyGUI::newDelegate(this, &PropertyTexturesControl::notifyEndDialog);
		mTextureBrowseControl->setTextures(mTextures);

		mComboBox->eventComboChangePosition += MyGUI::newDelegate(this, &PropertyTexturesControl::notifyComboChangePosition);
		mBrowse->eventMouseButtonClick += MyGUI::newDelegate(this, &PropertyTexturesControl::notifyMouseButtonClick);
	}

	PropertyTexturesControl::~PropertyTexturesControl()
	{
		mBrowse->eventMouseButtonClick -= MyGUI::newDelegate(this, &PropertyTexturesControl::notifyMouseButtonClick);
		mComboBox->eventComboChangePosition -= MyGUI::newDelegate(this, &PropertyTexturesControl::notifyComboChangePosition);

		delete mTextureBrowseControl;
		mTextureBrowseControl = nullptr;
	}

	void PropertyTexturesControl::updateProperty()
	{
		Property* proper = getProperty();
		if (proper != nullptr)
		{
			mComboBox->setEnabled(true);
			mBrowse->setEnabled(true);

			size_t index = getComboIndex(proper->getValue());
			mComboBox->setIndexSelected(index);
		}
		else
		{
			mComboBox->setIndexSelected(MyGUI::ITEM_NONE);
			mComboBox->setEnabled(false);
			mBrowse->setEnabled(false);
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

	void PropertyTexturesControl::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		Property* proper = getProperty();
		if (proper != nullptr)
			mTextureBrowseControl->setTextureName(proper->getValue());
		else
			mTextureBrowseControl->setTextureName("");

		mTextureBrowseControl->doModal();
	}

	void PropertyTexturesControl::notifyEndDialog(Dialog* _sender, bool _result)
	{
		mTextureBrowseControl->endModal();

		if (_result)
		{
			Property* proper = getProperty();
			if (proper != nullptr)
				proper->setValue(mTextureBrowseControl->getTextureName(), getTypeName());
			updateProperty();
		}
	}

	void PropertyTexturesControl::fillTextures()
	{
		// FIXME потом вынести в загружаемые настройки
		MyGUI::VectorString paths = MyGUI::DataManager::getInstance().getDataListNames("*.png");
		for (MyGUI::VectorString::iterator iter = paths.begin(); iter != paths.end(); ++iter)
			mTextures.push_back(*iter);

		// FIXME потом вынести в загружаемые настройки
		paths = MyGUI::DataManager::getInstance().getDataListNames("*.jpg");
		for (MyGUI::VectorString::iterator iter = paths.begin(); iter != paths.end(); ++iter)
			mTextures.push_back(*iter);
	}
} // namespace tools
