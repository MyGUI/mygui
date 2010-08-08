/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinPropertyControl.h"
#include "SkinManager.h"

namespace tools
{

	SkinPropertyControl::SkinPropertyControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SkinPropertyControl.layout", _parent),
		mTypeName("SkinPropertyControl"),
		mTextures(nullptr),
		mCoord(nullptr),
		mCurrentSkin(nullptr)
	{
		assignWidget(mTextures, "Textures");
		assignWidget(mCoord, "Coord");

		mTextures->eventComboChangePosition += MyGUI::newDelegate(this, &SkinPropertyControl::notifyComboChangePosition);
		mCoord->eventEditTextChange += MyGUI::newDelegate(this, &SkinPropertyControl::notifyEditTextChange);

		fillTextures();

		SkinManager::getInstance().eventChangeSelection += MyGUI::newDelegate(this, &SkinPropertyControl::notifyChangeSelection);
		advice();

		updateAllProperties();
	}

	SkinPropertyControl::~SkinPropertyControl()
	{
		mTextures->eventComboChangePosition -= MyGUI::newDelegate(this, &SkinPropertyControl::notifyComboChangePosition);
		mCoord->eventEditTextChange -= MyGUI::newDelegate(this, &SkinPropertyControl::notifyEditTextChange);

		SkinManager::getInstance().eventChangeSelection -= MyGUI::newDelegate(this, &SkinPropertyControl::notifyChangeSelection);
		unadvice();
	}

	void SkinPropertyControl::fillTextures()
	{
		mTextures->removeAllItems();

		MyGUI::VectorString paths = MyGUI::DataManager::getInstance().getDataListNames("*.png");
		for (MyGUI::VectorString::iterator iter=paths.begin(); iter!=paths.end(); ++iter)
			mTextures->addItem(*iter);

		paths = MyGUI::DataManager::getInstance().getDataListNames("*.jpg");
		for (MyGUI::VectorString::iterator iter=paths.begin(); iter!=paths.end(); ++iter)
			mTextures->addItem(*iter);
	}

	void SkinPropertyControl::notifyChangeSelection()
	{
		unadvice();
		advice();

		updateAllProperties();
	}

	void SkinPropertyControl::unadvice()
	{
		if (mCurrentSkin != nullptr)
		{
			mCurrentSkin->getPropertySet()->eventChangeProperty -= MyGUI::newDelegate(this, &SkinPropertyControl::notifyChangeProperty);
			mCurrentSkin = nullptr;
		}
	}

	void SkinPropertyControl::advice()
	{
		mCurrentSkin = SkinManager::getInstance().getItemSelected();

		if (mCurrentSkin != nullptr)
		{
			mCurrentSkin->getPropertySet()->eventChangeProperty += MyGUI::newDelegate(this, &SkinPropertyControl::notifyChangeProperty);
		}
	}

	void SkinPropertyControl::notifyChangeProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_owner != mTypeName)
		{
			if (_sender->getName() == "Texture")
				updateTexture();
			else if (_sender->getName() == "Coord")
				updateCoord();
		}
	}

	void SkinPropertyControl::updateAllProperties()
	{
		updateTexture();
		updateCoord();
	}

	void SkinPropertyControl::updateTexture()
	{
		size_t index = MyGUI::ITEM_NONE;

		if (mCurrentSkin != nullptr)
		{
			MyGUI::UString texture;

			Property* prop = mCurrentSkin->getPropertySet()->getChild("Texture");
			if (prop != nullptr)
				texture = prop->getValue();

			index = getComboIndex(texture);
		}

		mTextures->setIndexSelected(index);
	}

	size_t SkinPropertyControl::getComboIndex(const MyGUI::UString& _name)
	{
		size_t result = MyGUI::ITEM_NONE;

		size_t count = mTextures->getItemCount();
		for (size_t index=0; index<count; ++index)
		{
			if (mTextures->getItemNameAt(index) == _name)
			{
				result = index;
				break;
			}
		}

		return result;
	}

	void SkinPropertyControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		if (mCurrentSkin != nullptr)
		{
			Property* prop = mCurrentSkin->getPropertySet()->getChild("Texture");
			if (prop != nullptr)
			{
				if (_index != MyGUI::ITEM_NONE)
					prop->setValue(_sender->getItemNameAt(_index), mTypeName);
				else
					prop->setValue("", mTypeName);
			}
		}
	}

	void SkinPropertyControl::updateCoord()
	{
		MyGUI::UString value;

		if (mCurrentSkin != nullptr)
		{
			Property* prop = mCurrentSkin->getPropertySet()->getChild("Coord");
			if (prop != nullptr)
				value = prop->getValue();
		}

		mCoord->setCaption(value);

		bool validate = isCoordValidate();
		setColourCoord(validate);
	}

	void SkinPropertyControl::notifyEditTextChange(MyGUI::Edit* _sender)
	{
		if (mCurrentSkin != nullptr)
		{
			Property* prop = mCurrentSkin->getPropertySet()->getChild("Coord");
			if (prop != nullptr)
			{
				bool validate = isCoordValidate();
				if (validate)
					prop->setValue(getClearCoordValue(), mTypeName);

				setColourCoord(validate);
			}
		}
		else
		{
			mCoord->setCaption("");
		}
	}

	bool SkinPropertyControl::isCoordValidate()
	{
		MyGUI::UString value = mCoord->getOnlyText();

		MyGUI::IntCoord coord;
		if (!MyGUI::utility::parseComplex(value, coord.left, coord.top, coord.width, coord.height))
			return false;

		return true;
	}

	MyGUI::UString SkinPropertyControl::getClearCoordValue()
	{
		MyGUI::UString value = mCoord->getOnlyText();
		MyGUI::IntCoord coord;
		MyGUI::utility::parseComplex(value, coord.left, coord.top, coord.width, coord.height);
		return coord.print();
	}

	void SkinPropertyControl::setColourCoord(bool _validate)
	{
		MyGUI::UString value = mCoord->getOnlyText();
		if (!_validate)
			value = "#FF0000" + value;

		size_t index = mCoord->getTextCursor();
		mCoord->setCaption(value);
		mCoord->setTextCursor(index);
	}

} // namespace tools
