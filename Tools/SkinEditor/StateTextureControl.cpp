/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "StateTextureControl.h"
#include "SkinManager.h"

namespace tools
{

	StateTextureControl::StateTextureControl(MyGUI::Widget* _parent) :
		TextureControl(_parent),
		mTypeName("StateTextureControl"),
		mRegionSelectorControl(nullptr)
	{
		assignBase(mRegionSelectorControl, "Texture");

		mRegionSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &StateTextureControl::notifyChangePosition);

		initialiseAdvisor();
	}

	StateTextureControl::~StateTextureControl()
	{
		shutdownAdvisor();

		mRegionSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &StateTextureControl::notifyChangePosition);
	}

	void StateTextureControl::updateVisible()
	{
		MyGUI::UString visible;

		if (getCurrentState() != nullptr)
		{
			Property* prop = getCurrentState()->getPropertySet()->getChild("Visible");
			if (prop != nullptr)
				visible = prop->getValue();
		}

		mRegionSelectorControl->setVisible(visible == "True");
	}

	void StateTextureControl::updatePosition()
	{
		MyGUI::UString value;

		if (getCurrentState() != nullptr)
		{
			Property* prop = getCurrentState()->getPropertySet()->getChild("Position");
			if (prop != nullptr)
				value = prop->getValue();
		}

		MyGUI::IntPoint position;
		if (MyGUI::utility::parseComplex(value, position.left, position.top))
		{
			mRegionSelectorControl->setPosition(position);
		}
	}

	void StateTextureControl::updateTexture()
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

	void StateTextureControl::updateCoord()
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

	void StateTextureControl::updateRegionCoord()
	{
		mRegionSelectorControl->setSize(mCoordValue.size());
	}

	void StateTextureControl::notifyChangePosition()
	{
		MyGUI::IntPoint point = mRegionSelectorControl->getPosition();

		if (getCurrentState() != nullptr)
		{
			Property* prop = getCurrentState()->getPropertySet()->getChild("Position");
			if (prop != nullptr)
				prop->setValue(point.print(), mTypeName);
		}
	}

	void StateTextureControl::updateSkinProperties()
	{
		updateTexture();
		updateCoord();
	}

	void StateTextureControl::updateStateProperties()
	{
		updateVisible();
		updatePosition();
	}

	void StateTextureControl::updateSkinProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_owner != mTypeName)
		{
			if (_sender->getName() == "Texture")
				updateTexture();
			else if (_sender->getName() == "Coord")
				updateCoord();
		}
	}

	void StateTextureControl::updateStateProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_owner != mTypeName)
		{
			if (_sender->getName() == "Visible")
				updateVisible();
			else if (_sender->getName() == "Position")
				updatePosition();
		}
	}

} // namespace tools
