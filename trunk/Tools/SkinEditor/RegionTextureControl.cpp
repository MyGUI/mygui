/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "RegionTextureControl.h"

namespace tools
{

	RegionTextureControl::RegionTextureControl(MyGUI::Widget* _parent) :
		TextureControl(_parent),
		mTextureVisible(false),
		mAreaSelectorControl(nullptr),
		mPositionSelectorControl(nullptr)
	{
		mTypeName = MyGUI::utility::toString((int)this);

		// сразу рисуем рамки для стейтов
		std::vector<MyGUI::IntCoord> coords(10);
		drawUnselectedStates(coords);

		addSelectorControl(mAreaSelectorControl);
		addSelectorControl(mPositionSelectorControl);

		mPositionSelectorControl->setEnabled(false);

		mAreaSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &RegionTextureControl::notifyChangePosition);

		initialiseAdvisor();
	}

	RegionTextureControl::~RegionTextureControl()
	{
		shutdownAdvisor();

		mAreaSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &RegionTextureControl::notifyChangePosition);
	}

	void RegionTextureControl::updateRegionProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_owner != mTypeName)
		{
			if (_sender->getName() == "Position")
				updatePosition();
			else if (_sender->getName() == "Visible")
				updateVisible();
			else if (_sender->getName() == "Enabled")
				updateVisible();
		}
	}

	void RegionTextureControl::updateRegionProperties()
	{
		updateVisible();
		updatePosition();

		updateUnselectedStates();
	}

	void RegionTextureControl::updateTextureControl()
	{
		if (mTextureVisible && !mTextureName.empty() && getCurrentSkin() != nullptr && getCurrentState() != nullptr)
		{
			setTextureName(mTextureName);
			setTextureRegion(mTextureRegion);
		}
		else
		{
			setTextureRegion(MyGUI::IntCoord());
		}
	}

	void RegionTextureControl::updateSkinProperties()
	{
		updateTextureName();
		updateRegionSize();
	}

	void RegionTextureControl::updateSkinProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Texture")
			updateTextureName();
		else if (_sender->getName() == "Coord")
			updateRegionSize();
	}

	void RegionTextureControl::updateStateProperties()
	{
		updateRegionPosition();
		updateTextureVisible();
	}

	void RegionTextureControl::updateStateProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Position")
			updateRegionPosition();
		else if (_sender->getName() == "Visible")
			updateTextureVisible();
	}

	void RegionTextureControl::updateTextureName()
	{
		mTextureName = "";
		if (getCurrentSkin() != nullptr)
			mTextureName = getCurrentSkin()->getPropertySet()->getPropertyValue("Texture");

		updateTextureControl();
	}

	void RegionTextureControl::updateRegionSize()
	{
		mTextureRegion.width = 0;
		mTextureRegion.height = 0;

		if (getCurrentSkin() != nullptr)
		{
			MyGUI::IntCoord coord = MyGUI::IntCoord::parse(getCurrentSkin()->getPropertySet()->getPropertyValue("Coord"));
			mTextureRegion.width = coord.width;
			mTextureRegion.height = coord.height;
		}

		updateTextureControl();

		updateUnselectedStates();
	}

	void RegionTextureControl::updateRegionPosition()
	{
		mTextureRegion.left = 0;
		mTextureRegion.top = 0;

		if (getCurrentState() != nullptr)
		{
			MyGUI::IntPoint position = MyGUI::IntPoint::parse(getCurrentState()->getPropertySet()->getPropertyValue("Position"));
			mTextureRegion.left = position.left;
			mTextureRegion.top = position.top;
		}

		updateTextureControl();
	}

	void RegionTextureControl::updateTextureVisible()
	{
		mTextureVisible = false;

		if (getCurrentState() != nullptr)
			mTextureVisible = getCurrentState()->getPropertySet()->getPropertyValue("Visible") == "True";

		updateTextureControl();
	}

	void RegionTextureControl::updatePosition()
	{
		MyGUI::UString value;

		if (getCurrentRegion() != nullptr)
			value = getCurrentRegion()->getPropertySet()->getPropertyValue("Position");

		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(value, coord.left, coord.top, coord.width, coord.height))
		{
			mAreaSelectorControl->setCoord(coord);
			mPositionSelectorControl->setCoord(coord);
		}
	}

	void RegionTextureControl::updateVisible()
	{
		mAreaSelectorControl->setVisible(false);
		mPositionSelectorControl->setVisible(false);

		if (getCurrentRegion() != nullptr)
		{
			if (getCurrentRegion()->getPropertySet()->getPropertyValue("Visible") == "True")
			{
				if (getCurrentRegion()->getPropertySet()->getPropertyValue("Enabled") == "True")
				{
					if (!getCurrentRegion()->getPropertySet()->getPropertyReadOnly("Position"))
						mAreaSelectorControl->setVisible(true);
					else
						mPositionSelectorControl->setVisible(true);
				}
			}
		}
	}

	void RegionTextureControl::notifyChangePosition()
	{
		MyGUI::IntCoord coord = mAreaSelectorControl->getCoord();

		if (getCurrentRegion() != nullptr)
			getCurrentRegion()->getPropertySet()->setPropertyValue("Position", coord.print(), mTypeName);
	}

	void RegionTextureControl::updateUnselectedStates()
	{
		std::vector<MyGUI::IntCoord> coords;

		if (getCurrentSkin() != nullptr)
		{
			ItemHolder<RegionItem>::EnumeratorItem regions = getCurrentSkin()->getRegions().getChildsEnumerator();
			while (regions.next())
			{
				RegionItem* item = regions.current();
				if (item != getCurrentSkin()->getRegions().getItemSelected())
				{
					if (item->getPropertySet()->getPropertyValue("Visible") == "True")
					{
						if (item->getPropertySet()->getPropertyValue("Enabled") == "True")
						{
							addCoord(coords, item->getPropertySet()->getPropertyValue("Position"));
						}
					}
				}
			}
		}

		drawUnselectedStates(coords);
	}

	void RegionTextureControl::addCoord(std::vector<MyGUI::IntCoord>& _coords, const MyGUI::UString& _coord)
	{
		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(_coord, coord.left, coord.top, coord.width, coord.height))
		{
			for (std::vector<MyGUI::IntCoord>::iterator item=_coords.begin(); item!=_coords.end(); ++item)
			{
				if ((*item) == coord)
					return;
			}

			_coords.push_back(coord);
		}
	}

	void RegionTextureControl::drawUnselectedStates(std::vector<MyGUI::IntCoord>& _coords)
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

	void RegionTextureControl::updateSeparatorProperties()
	{
		updateUnselectedStates();
	}

	void RegionTextureControl::updateSeparatorProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Visible" || _sender->getName() == "Position")
			updateUnselectedStates();
	}

} // namespace tools
