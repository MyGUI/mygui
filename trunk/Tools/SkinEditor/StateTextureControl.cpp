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
		mRegionSelectorControl(nullptr)
	{
		mTypeName = MyGUI::utility::toString((int)this);

		// ����� ������ ����� ��� �������
		std::vector<MyGUI::IntCoord> coords(8);
		drawUnselectedStates(coords);

		addSelectorControl(mRegionSelectorControl);

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
			visible = getCurrentState()->getPropertySet()->getPropertyValue("Visible");

		mRegionSelectorControl->setVisible(visible == "True");
	}

	void StateTextureControl::updatePosition()
	{
		MyGUI::UString value;

		if (getCurrentState() != nullptr)
			value = getCurrentState()->getPropertySet()->getPropertyValue("Position");

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
			texture = getCurrentSkin()->getPropertySet()->getPropertyValue("Texture");

		setTextureName(texture);
	}

	void StateTextureControl::updateCoord()
	{
		MyGUI::UString value;

		if (getCurrentSkin() != nullptr)
			value = getCurrentSkin()->getPropertySet()->getPropertyValue("Coord");

		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(value, coord.left, coord.top, coord.width, coord.height))
		{
			mCoordValue = coord;
			updateRegionCoord();
		}
	}

	void StateTextureControl::updateRegionCoord()
	{
		mRegionSelectorControl->setSize(mCoordValue.size());

		updateUnselectedStates();
	}

	void StateTextureControl::notifyChangePosition()
	{
		MyGUI::IntPoint point = mRegionSelectorControl->getPosition();

		if (getCurrentState() != nullptr)
			getCurrentState()->getPropertySet()->setPropertyValue("Position", point.print(), mTypeName);
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

		updateUnselectedStates();
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

	void StateTextureControl::updateUnselectedStates()
	{
		std::vector<MyGUI::IntCoord> coords;

		if (getCurrentSkin() != nullptr)
		{
			ItemHolder<StateItem>::EnumeratorItem states = getCurrentSkin()->getStates().getChildsEnumerator();
			while (states.next())
			{
				StateItem* item = states.current();
				if (item != getCurrentSkin()->getStates().getItemSelected())
				{
					if (item->getPropertySet()->getPropertyValue("Visible") == "True")
						addCoord(coords,
							getCurrentSkin()->getPropertySet()->getPropertyValue("Coord"),
							item->getPropertySet()->getPropertyValue("Position"));
				}
			}
		}

		drawUnselectedStates(coords);
	}

	void StateTextureControl::addCoord(std::vector<MyGUI::IntCoord>& _coords, const MyGUI::UString& _coord, const MyGUI::UString& _position)
	{
		MyGUI::IntCoord coord;
		MyGUI::IntPoint point;
		if (MyGUI::utility::parseComplex(_coord, coord.left, coord.top, coord.width, coord.height))
		{
			if (MyGUI::utility::parseComplex(_position, point.left, point.top))
			{
				coord = point;

				for (std::vector<MyGUI::IntCoord>::iterator item=_coords.begin(); item!=_coords.end(); ++item)
				{
					if ((*item) == coord)
						return;
				}

				_coords.push_back(coord);
			}
		}
	}

	void StateTextureControl::drawUnselectedStates(std::vector<MyGUI::IntCoord>& _coords)
	{
		while (_coords.size() > mBlackSelectors.size())
		{
			PositionSelectorBlackControl* selector = nullptr;
			addSelectorControl(selector);

			mBlackSelectors.push_back(selector);
		}

		for (size_t index=0; index<mBlackSelectors.size(); ++index)
		{
			if (index < _coords.size())
			{
				mBlackSelectors[index]->setVisible(true);
				mBlackSelectors[index]->setCoord(_coords[index]);
			}
			else
			{
				mBlackSelectors[index]->setVisible(false);
			}
		}
	}

} // namespace tools
