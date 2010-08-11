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
		mAreaSelectorControl(nullptr)
	{
		mTypeName = MyGUI::utility::toString((int)this);

		addSelectorControl(mAreaSelectorControl);

		mAreaSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &SkinTextureControl::notifyChangePosition);

		initialiseAdvisor();
	}

	SkinTextureControl::~SkinTextureControl()
	{
		shutdownAdvisor();

		mAreaSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &SkinTextureControl::notifyChangePosition);
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
			texture = getCurrentSkin()->getPropertySet()->getPropertyValue("Texture");

		setTextureName(texture);
	}

	void SkinTextureControl::updateCoord()
	{
		MyGUI::UString value;

		if (getCurrentSkin() != nullptr)
			value = getCurrentSkin()->getPropertySet()->getPropertyValue("Coord");

		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(value, coord.left, coord.top, coord.width, coord.height))
		{
			mCoordValue = coord;
			mAreaSelectorControl->setVisible(true);
			updateRegionCoord();
		}
		else
		{
			mAreaSelectorControl->setVisible(false);
		}
	}

	void SkinTextureControl::updateRegionCoord()
	{
		mAreaSelectorControl->setCoord(mCoordValue);
	}

	void SkinTextureControl::notifyChangePosition()
	{
		mCoordValue = mAreaSelectorControl->getCoord();

		if (getCurrentSkin() != nullptr)
			getCurrentSkin()->getPropertySet()->setPropertyValue("Coord", mCoordValue.print(), mTypeName);
	}

} // namespace tools
