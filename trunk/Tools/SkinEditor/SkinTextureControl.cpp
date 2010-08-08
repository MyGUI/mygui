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
		mTypeName("SkinTextureControl"),
		mView(nullptr),
		mTexture(nullptr),
		mBackgroundColour(nullptr),
		mBackground(nullptr),
		mRegionSelectorControl(nullptr),
		mScale(nullptr),
		mScaleValue(1)
	{
		assignWidget(mView, "View");
		assignWidget(mTexture, "Texture");
		assignWidget(mBackgroundColour, "BackgroundColour");
		assignWidget(mBackground, "Background");
		assignWidget(mScale, "Scale");
		assignBase(mRegionSelectorControl, "Texture");

		fillColours(mBackgroundColour);
		mBackgroundColour->eventComboChangePosition += MyGUI::newDelegate(this, &SkinTextureControl::notifyComboChangePosition);

		fillScale();
		mScale->eventComboChangePosition += MyGUI::newDelegate(this, &SkinTextureControl::notifyComboChangePosition);

		mRegionSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &SkinTextureControl::notifyChangePosition);

		initialiseAdvisor();
	}

	SkinTextureControl::~SkinTextureControl()
	{
		shutdownAdvisor();

		mRegionSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &SkinTextureControl::notifyChangePosition);

		mBackgroundColour->eventComboChangePosition -= MyGUI::newDelegate(this, &SkinTextureControl::notifyComboChangePosition);
		mScale->eventComboChangePosition -= MyGUI::newDelegate(this, &SkinTextureControl::notifyComboChangePosition);
	}

	void SkinTextureControl::updateSkinProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_owner != mTypeName)
		{
			if (_sender->getName() == "Texture")
				updateTexture();
			else if (_sender->getName() == "Coord")
				updateCoord();
		}
	}

	void SkinTextureControl::updateSkinProperties()
	{
		updateTexture();
		updateCoord();
	}

	void SkinTextureControl::updateTexture()
	{
		MyGUI::UString texture;

		if (getCurrentSkin() != nullptr)
		{
			Property* prop = getCurrentSkin()->getPropertySet()->getChild("Texture");
			if (prop != nullptr)
				texture = prop->getValue();
		}

		mTextureSize = MyGUI::texture_utility::getTextureSize(texture);
		mTexture->setImageTexture(texture);

		updateScale();
	}

	void SkinTextureControl::updateCoord()
	{
		MyGUI::UString value;

		if (getCurrentSkin() != nullptr)
		{
			Property* prop = getCurrentSkin()->getPropertySet()->getChild("Coord");
			if (prop != nullptr)
				value = prop->getValue();
		}

		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(value, coord.left, coord.top, coord.width, coord.height))
		{
			mCoordValue = coord;
			mRegionSelectorControl->setVisible(true);
			updateRegionCoord();
		}
		else
		{
			mRegionSelectorControl->setVisible(false);
		}
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
		else if (_sender == mScale)
		{
			size_t index = mScale->getIndexSelected();
			if (index != MyGUI::ITEM_NONE)
			{
				mScaleValue = *mScale->getItemDataAt<double>(index);
				updateScale();
			}
		}
	}

	void SkinTextureControl::fillScale()
	{
		mScale->removeAllItems();

		mScale->addItem("50 %", (double)0.5);
		mScale->addItem("100 %", (double)1);
		mScale->addItem("200 %", (double)2);
		mScale->addItem("400 %", (double)4);
		mScale->addItem("800 %", (double)8);
		mScale->addItem("1600 %", (double)16);

		size_t index = 1;

		mScale->setIndexSelected(index);
		mScaleValue = *mScale->getItemDataAt<double>(index);

		updateScale();
	}

	void SkinTextureControl::updateScale()
	{
		double width = (double)mTextureSize.width * mScaleValue;
		double height = (double)mTextureSize.height * mScaleValue;

		mView->setCanvasSize(MyGUI::IntSize((int)width, (int)height));
		mRegionSelectorControl->setScale(mScaleValue);
	}

	void SkinTextureControl::updateRegionCoord()
	{
		mRegionSelectorControl->setCoord(mCoordValue);
	}

	void SkinTextureControl::notifyChangePosition()
	{
		mCoordValue = mRegionSelectorControl->getCoord();

		if (getCurrentSkin() != nullptr)
		{
			Property* prop = getCurrentSkin()->getPropertySet()->getChild("Coord");
			if (prop != nullptr)
				prop->setValue(mCoordValue.print(), mTypeName);
		}
	}

} // namespace tools
