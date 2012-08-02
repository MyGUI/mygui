/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyTexturesControl.h"
#include "Localise.h"

namespace tools
{

	PropertyTexturesControl::PropertyTexturesControl() :
		mImage(nullptr),
		mName(nullptr),
		mComboBox(nullptr),
		mBrowse(nullptr),
		mTextureBrowseControl(nullptr)
	{
	}

	PropertyTexturesControl::~PropertyTexturesControl()
	{
		mBrowse->eventMouseButtonClick -= MyGUI::newDelegate(this, &PropertyTexturesControl::notifyMouseButtonClick);
		mComboBox->eventComboChangePosition -= MyGUI::newDelegate(this, &PropertyTexturesControl::notifyComboChangePosition);

		delete mTextureBrowseControl;
		mTextureBrowseControl = nullptr;
	}

	void PropertyTexturesControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		PropertyControl::OnInitialise(_parent, _place, "PropertyTextureBrowseControl.layout");

		assignWidget(mName, "Name", false);
		assignWidget(mComboBox, "ComboBox");
		assignWidget(mBrowse, "Browse");
		assignWidget(mImage, "Image");

		fillTextures();

		for (MyGUI::VectorString::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
			mComboBox->addItem((*item));

		mComboBox->beginToItemFirst();

		mTextureBrowseControl = new TextureBrowseControl();
		mTextureBrowseControl->Initialise();
		mTextureBrowseControl->eventEndDialog.connect(this, &PropertyTexturesControl::notifyEndDialog);
		mTextureBrowseControl->setTextures(mTextures);

		mComboBox->eventComboChangePosition += MyGUI::newDelegate(this, &PropertyTexturesControl::notifyComboChangePosition);
		mBrowse->eventMouseButtonClick += MyGUI::newDelegate(this, &PropertyTexturesControl::notifyMouseButtonClick);

		mMainWidget->eventChangeCoord += MyGUI::newDelegate(this, &PropertyTexturesControl::notifyChangeCoord);
	}

	void PropertyTexturesControl::updateCaption()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
			mName->setCaption(proper->getType()->getName());
	}

	void PropertyTexturesControl::updateProperty()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			mComboBox->setEnabled(!proper->getType()->getReadOnly());
			mBrowse->setEnabled(!proper->getType()->getReadOnly());

			size_t index = getComboIndex(proper->getValue());
			mComboBox->setIndexSelected(index);
			if (index == MyGUI::ITEM_NONE)
				mComboBox->setCaption(replaceTags("ColourError") + proper->getValue());

			mImage->setVisible(true);
			mImage->setImageTexture(proper->getValue());

			mTextureSize = MyGUI::texture_utility::getTextureSize(proper->getValue());

			updateTexture();
		}
		else
		{
			mComboBox->setIndexSelected(MyGUI::ITEM_NONE);
			mComboBox->setEnabled(false);
			mBrowse->setEnabled(false);
			mImage->setVisible(false);
		}
	}

	void PropertyTexturesControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			std::string value = _index != MyGUI::ITEM_NONE ? mComboBox->getItemNameAt(_index) : "";
			executeAction(value);
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
		PropertyPtr proper = getProperty();
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
			PropertyPtr proper = getProperty();
			if (proper != nullptr)
				executeAction(mTextureBrowseControl->getTextureName());
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

	void PropertyTexturesControl::updateTexture()
	{
		if (mTextureSize.width != 0 && mTextureSize.height != 0)
		{
			mImage->setVisible(true);

			const MyGUI::IntSize& targetSize = mImage->getParentSize();

			float k1 = (float)targetSize.width / mTextureSize.width;
			float k2 = (float)targetSize.height / mTextureSize.height;
			float k = (std::min)(k1, k2);
			MyGUI::IntSize size = MyGUI::IntSize((int)(k * (float)mTextureSize.width), (int)(k * (float)mTextureSize.height));

			MyGUI::IntSize parentSize = mImage->getParentSize();
			mImage->setCoord((parentSize.width - size.width) / 2, (parentSize.height - size.height) / 2, size.width, size.height);
		}
		else
		{
			mImage->setVisible(false);
		}
	}

	void PropertyTexturesControl::notifyChangeCoord(MyGUI::Widget* _sender)
	{
		if (mImage->getVisible() && mImage->getInheritedVisible())
			updateTexture();
	}

}
