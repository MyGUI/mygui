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
		TextureControl(_parent),
		mTypeName("SkinTextureControl"),
		mRegionSelectorControl(nullptr)
	{
		assignBase(mRegionSelectorControl, "Texture");

		mRegionSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &SkinTextureControl::notifyChangePosition);

		initialiseAdvisor();
	}

	SkinTextureControl::~SkinTextureControl()
	{
		shutdownAdvisor();

		mRegionSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &SkinTextureControl::notifyChangePosition);
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

		setTextureName(texture);
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
