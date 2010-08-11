/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SeparatorTextureControl.h"

namespace tools
{

	SeparatorTextureControl::SeparatorTextureControl(MyGUI::Widget* _parent) :
		TextureControl(_parent),
		mTextureVisible(false),
		mHorizontalSelectorControl(nullptr),
		mVerticalSelectorControl(nullptr),
		mHorizontal(false)
	{
		mTypeName = MyGUI::utility::toString((int)this);

		// сразу рисуем рамки для стейтов
		std::vector<int> coordsHor(2);
		std::vector<int> coordsVert(2);
		drawUnselectedStates(coordsHor, coordsVert);

		addSelectorControl(mHorizontalSelectorControl);
		addSelectorControl(mVerticalSelectorControl);

		mHorizontalSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &SeparatorTextureControl::notifyChangePosition);
		mVerticalSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &SeparatorTextureControl::notifyChangePosition);

		initialiseAdvisor();
	}

	SeparatorTextureControl::~SeparatorTextureControl()
	{
		shutdownAdvisor();

		mHorizontalSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &SeparatorTextureControl::notifyChangePosition);
		mVerticalSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &SeparatorTextureControl::notifyChangePosition);
	}

	void SeparatorTextureControl::updateSeparatorProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_owner != mTypeName)
		{
			if (_sender->getName() == "Position")
				updatePosition();
			else if (_sender->getName() == "Visible")
				updateVisible();
		}
	}

	void SeparatorTextureControl::updateSeparatorProperties()
	{
		updateVisible();
		updatePosition();

		updateUnselectedStates();
	}

	void SeparatorTextureControl::updateTextureControl()
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

	void SeparatorTextureControl::updateSkinProperties()
	{
		updateTextureName();
		updateRegionSize();
	}

	void SeparatorTextureControl::updateSkinProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Texture")
			updateTextureName();
		else if (_sender->getName() == "Coord")
			updateRegionSize();
	}

	void SeparatorTextureControl::updateStateProperties()
	{
		updateRegionPosition();
		updateTextureVisible();
	}

	void SeparatorTextureControl::updateStateProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Position")
			updateRegionPosition();
		else if (_sender->getName() == "Visible")
			updateTextureVisible();
	}

	void SeparatorTextureControl::updateTextureName()
	{
		mTextureName = "";
		if (getCurrentSkin() != nullptr)
			mTextureName = getCurrentSkin()->getPropertySet()->getPropertyValue("Texture");

		updateTextureControl();
	}

	void SeparatorTextureControl::updateRegionSize()
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

	void SeparatorTextureControl::updateRegionPosition()
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

	void SeparatorTextureControl::updateTextureVisible()
	{
		mTextureVisible = false;

		if (getCurrentState() != nullptr)
			mTextureVisible = getCurrentState()->getPropertySet()->getPropertyValue("Visible") == "True";

		updateTextureControl();
	}

	void SeparatorTextureControl::updatePosition()
	{
		MyGUI::UString value;

		if (getCurrentSeparator() != nullptr)
			value = getCurrentSeparator()->getPropertySet()->getPropertyValue("Position");

		int position = 0;
		if (MyGUI::utility::parseComplex(value, position))
		{
			if (mHorizontal)
				mHorizontalSelectorControl->setCoord(MyGUI::IntCoord(0, position, mTextureRegion.width, 1));
			else
				mVerticalSelectorControl->setCoord(MyGUI::IntCoord(position, 0, 1, mTextureRegion.height));
		}
	}

	void SeparatorTextureControl::updateVisible()
	{
		mHorizontalSelectorControl->setVisible(false);
		mVerticalSelectorControl->setVisible(false);

		if (getCurrentSeparator() != nullptr)
		{
			mHorizontal = getCurrentSeparator()->getCorner() == MyGUI::Align::Left || getCurrentSeparator()->getCorner() == MyGUI::Align::Right;

			if (getCurrentSeparator()->getPropertySet()->getPropertyValue("Visible") == "True")
			{
				if (mHorizontal)
					mHorizontalSelectorControl->setVisible(true);
				else
					mVerticalSelectorControl->setVisible(true);
			}
		}
	}

	void SeparatorTextureControl::notifyChangePosition()
	{
		int position = 0;
		if (mHorizontal)
			position = mHorizontalSelectorControl->getPosition().top;
		else
			position = mVerticalSelectorControl->getPosition().left;

		if (getCurrentSeparator() != nullptr)
			getCurrentSeparator()->getPropertySet()->setPropertyValue("Position", MyGUI::utility::toString(position), mTypeName);
	}

	void SeparatorTextureControl::updateUnselectedStates()
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
						bool horizontal =item->getCorner() == MyGUI::Align::Left || item->getCorner() == MyGUI::Align::Right;
						addCoord(coordsHor, coordsVert, horizontal,
							item->getPropertySet()->getPropertyValue("Position"));
					}
				}
			}
		}

		drawUnselectedStates(coordsHor, coordsVert);
	}

	void SeparatorTextureControl::addCoord(std::vector<int>& _coordsHor, std::vector<int>& _coordsVert, bool _horizont, const MyGUI::UString& _position)
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

	void SeparatorTextureControl::drawUnselectedStates(std::vector<int>& _coordsHor, std::vector<int>& _coordsVert)
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
	}

} // namespace tools
