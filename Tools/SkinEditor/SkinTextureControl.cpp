/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinTextureControl.h"
#include "SkinManager.h"

namespace tools
{

	SkinTextureControl::SkinTextureControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SkinTextureControl.layout", _parent),
		mView(nullptr),
		mTexture(nullptr),
		mBackgroundColour(nullptr),
		mBackground(nullptr),
		mCurrentSkin(nullptr),
		mTypeName("SkinTextureControl")
	{
		assignWidget(mView, "View");
		assignWidget(mTexture, "Texture");
		assignWidget(mBackgroundColour, "BackgroundColour");
		assignWidget(mBackground, "Background");

		fillColours(mBackgroundColour);
		mBackgroundColour->eventComboChangePosition += MyGUI::newDelegate(this, &SkinTextureControl::notifyComboChangePosition);

		SkinManager::getInstance().eventChangeSelection += MyGUI::newDelegate(this, &SkinTextureControl::notifyChangeSelection);
		advice();

		updateAllProperties();
	}

	SkinTextureControl::~SkinTextureControl()
	{
		mBackgroundColour->eventComboChangePosition -= MyGUI::newDelegate(this, &SkinTextureControl::notifyComboChangePosition);

		SkinManager::getInstance().eventChangeSelection -= MyGUI::newDelegate(this, &SkinTextureControl::notifyChangeSelection);
		unadvice();
	}

	void SkinTextureControl::notifyChangeSelection()
	{
		unadvice();
		advice();

		updateAllProperties();
	}

	void SkinTextureControl::unadvice()
	{
		if (mCurrentSkin != nullptr)
		{
			mCurrentSkin->getPropertySet()->eventChangeProperty -= MyGUI::newDelegate(this, &SkinTextureControl::notifyChangeProperty);
			mCurrentSkin = nullptr;
		}
	}

	void SkinTextureControl::advice()
	{
		mCurrentSkin = SkinManager::getInstance().getItemSelected();

		if (mCurrentSkin != nullptr)
		{
			mCurrentSkin->getPropertySet()->eventChangeProperty += MyGUI::newDelegate(this, &SkinTextureControl::notifyChangeProperty);
		}
	}

	void SkinTextureControl::notifyChangeProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_owner != mTypeName)
		{
			if (_sender->getName() == "Texture")
				updateTexture();
			//else if (_sender->getName() == "Coord")
			//	updateCoord();
		}
	}

	void SkinTextureControl::updateAllProperties()
	{
		updateTexture();
		//updateCoord();
	}

	void SkinTextureControl::updateTexture()
	{
		MyGUI::UString texture;

		if (mCurrentSkin != nullptr)
		{
			Property* prop = mCurrentSkin->getPropertySet()->getChild("Texture");
			if (prop != nullptr)
				texture = prop->getValue();
		}

		MyGUI::IntSize size = MyGUI::texture_utility::getTextureSize(texture);

		mTexture->setImageTexture(texture);
		mView->setCanvasSize(size);
	}

	void SkinTextureControl::fillColours(MyGUI::ComboBox* _combo)
	{
		_combo->removeAllItems();

		_combo->addItem("Zero", MyGUI::Colour::Zero);
		_combo->addItem("Black", MyGUI::Colour::Black);
		_combo->addItem("White", MyGUI::Colour::White);
		_combo->addItem("Red", MyGUI::Colour::Red);
		_combo->addItem("Green", MyGUI::Colour::Green);
		_combo->addItem("Blue", MyGUI::Colour::Blue);

		_combo->setIndexSelected(0);

		updateColour(_combo);
	}

	void SkinTextureControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		updateColour(_sender);
	}

	void SkinTextureControl::updateColour(MyGUI::ComboBox* _sender)
	{
		if (_sender == mBackgroundColour)
		{
			size_t index = mBackgroundColour->getIndexSelected();
			if (index != MyGUI::ITEM_NONE)
			{
				MyGUI::Colour colour = *mBackgroundColour->getItemDataAt<MyGUI::Colour>(index);
				mBackground->setColour(colour);
				mBackground->setAlpha(colour.alpha);
			}
		}
	}

} // namespace tools
