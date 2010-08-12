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
		//std::vector<int> coordsHor(2);
		//std::vector<int> coordsVert(2);
		//drawUnselectedStates(coordsHor, coordsVert);

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

		//updateUnselectedStates();
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

		//updateUnselectedStates();
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

	/*void RegionTextureControl::updateUnselectedStates()
	{
		std::vector<int> coordsHor;
		std::vector<int> coordsVert;

		if (getCurrentSkin() != nullptr)
		{
			ItemHolder<SeparatorItem>::EnumeratorItem separators = getCurrentSkin()->getSeparators().getChildsEnumerator();
			while (separators.next())
			{
				SeparatorItem* item = separators.current();
				if (item != getCurrentSkin()->getSeparators().getItemSelected())
				{
					if (item->getPropertySet()->getPropertyValue("Visible") == "True")
					{
						bool horizontal =item->getCorner() == MyGUI::Align::Top || item->getCorner() == MyGUI::Align::Bottom;
						addCoord(coordsHor, coordsVert, horizontal,
							item->getPropertySet()->getPropertyValue("Position"));
					}
				}
			}
		}

		drawUnselectedStates(coordsHor, coordsVert);
	}

	void RegionTextureControl::addCoord(std::vector<int>& _coordsHor, std::vector<int>& _coordsVert, bool _horizont, const MyGUI::UString& _position)
	{
		int position;
		if (MyGUI::utility::parseComplex(_position, position))
		{
			if (_horizont)
			{
				for (std::vector<int>::iterator item=_coordsHor.begin(); item!=_coordsHor.end(); ++item)
				{
					if ((*item) == position)
						return;
				}
				_coordsHor.push_back(position);
			}
			else
			{
				for (std::vector<int>::iterator item=_coordsVert.begin(); item!=_coordsVert.end(); ++item)
				{
					if ((*item) == position)
						return;
				}
				_coordsVert.push_back(position);
			}
		}
	}

	void RegionTextureControl::drawUnselectedStates(std::vector<int>& _coordsHor, std::vector<int>& _coordsVert)
	{
		while (_coordsHor.size() > mHorizontalBlackSelectors.size())
		{
			HorizontalSelectorBlackControl* selector = nullptr;
			addSelectorControl(selector);

			mHorizontalBlackSelectors.push_back(selector);
		}

		for (size_t index=0; index<mHorizontalBlackSelectors.size(); ++index)
		{
			if (index < _coordsHor.size())
			{
				mHorizontalBlackSelectors[index]->setVisible(true);
				mHorizontalBlackSelectors[index]->setCoord(MyGUI::IntCoord(0, _coordsHor[index], mTextureRegion.width, 1));
			}
			else
			{
				mHorizontalBlackSelectors[index]->setVisible(false);
			}
		}

		while (_coordsVert.size() > mVerticalBlackSelectors.size())
		{
			VerticalSelectorBlackControl* selector = nullptr;
			addSelectorControl(selector);

			mVerticalBlackSelectors.push_back(selector);
		}

		for (size_t index=0; index<mVerticalBlackSelectors.size(); ++index)
		{
			if (index < _coordsVert.size())
			{
				mVerticalBlackSelectors[index]->setVisible(true);
				mVerticalBlackSelectors[index]->setCoord(MyGUI::IntCoord(_coordsVert[index], 0, 1, mTextureRegion.height));
			}
			else
			{
				mVerticalBlackSelectors[index]->setVisible(false);
			}
		}
	}*/

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

} // namespace tools
