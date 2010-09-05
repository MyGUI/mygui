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
		mVerticalSelectorControl(nullptr)
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
			if (_sender->getName() == "Offset")
				updatePosition();
			else if (_sender->getName() == "Visible")
			{
				updateVisible();
				updateUnselectedStates();
			}
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
		if (getCurrentSeparator() != nullptr)
		{
			MyGUI::UString value = getCurrentSeparator()->getPropertySet()->getPropertyValue("Offset");
			MyGUI::Align corner = getCurrentSeparator()->getCorner();

			int offset = 0;
			if (MyGUI::utility::parseComplex(value, offset))
			{
				if (corner.isTop())
					mHorizontalSelectorControl->setCoord(MyGUI::IntCoord(0, offset, mTextureRegion.width, 1));
				else if (corner.isLeft())
					mVerticalSelectorControl->setCoord(MyGUI::IntCoord(offset, 0, 1, mTextureRegion.height));
				else if (corner.isBottom())
					mHorizontalSelectorControl->setCoord(MyGUI::IntCoord(0, mTextureRegion.height - offset, mTextureRegion.width, 1));
				else if (corner.isRight())
					mVerticalSelectorControl->setCoord(MyGUI::IntCoord(mTextureRegion.width - offset, 0, 1, mTextureRegion.height));
			}
		}
	}

	void SeparatorTextureControl::updateVisible()
	{
		mHorizontalSelectorControl->setVisible(false);
		mVerticalSelectorControl->setVisible(false);

		if (getCurrentSeparator() != nullptr)
		{
			MyGUI::Align corner = getCurrentSeparator()->getCorner();

			if (getCurrentSeparator()->getPropertySet()->getPropertyValue("Visible") == "True")
			{
				if (corner.isTop() || corner.isBottom())
					mHorizontalSelectorControl->setVisible(true);
				else
					mVerticalSelectorControl->setVisible(true);
			}
		}
	}

	void SeparatorTextureControl::notifyChangePosition()
	{
		if (getCurrentSeparator() != nullptr)
		{
			MyGUI::Align corner = getCurrentSeparator()->getCorner();

			if (corner.isTop())
				getCurrentSeparator()->getPropertySet()->setPropertyValue("Offset",
					MyGUI::utility::toString(mHorizontalSelectorControl->getPosition().top), mTypeName);
			else if (corner.isLeft())
				getCurrentSeparator()->getPropertySet()->setPropertyValue("Offset",
					MyGUI::utility::toString(mVerticalSelectorControl->getPosition().left), mTypeName);
			else if (corner.isBottom())
				getCurrentSeparator()->getPropertySet()->setPropertyValue("Offset",
					MyGUI::utility::toString(mTextureRegion.height - mHorizontalSelectorControl->getPosition().top), mTypeName);
			else if (corner.isRight())
				getCurrentSeparator()->getPropertySet()->setPropertyValue("Offset",
					MyGUI::utility::toString(mTextureRegion.width -  mVerticalSelectorControl->getPosition().left), mTypeName);
		}
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
						MyGUI::Align corner = item->getCorner();

						addCoord(coordsHor, coordsVert, corner,
							item->getPropertySet()->getPropertyValue("Offset"));
					}
				}
			}
		}

		drawUnselectedStates(coordsHor, coordsVert);
	}

	void SeparatorTextureControl::addCoord(std::vector<int>& _coordsHor, std::vector<int>& _coordsVert, MyGUI::Align _corner, const MyGUI::UString& _position)
	{
		int offset = 0;
		if (MyGUI::utility::parseComplex(_position, offset))
		{
			if (_corner.isTop())
			{
				for (std::vector<int>::iterator item = _coordsHor.begin(); item != _coordsHor.end(); ++item)
				{
					if ((*item) == offset)
						return;
				}
				_coordsHor.push_back(offset);
			}
			else if (_corner.isLeft())
			{
				for (std::vector<int>::iterator item = _coordsVert.begin(); item != _coordsVert.end(); ++item)
				{
					if ((*item) == offset)
						return;
				}
				_coordsVert.push_back(offset);
			}
			else if (_corner.isBottom())
			{
				for (std::vector<int>::iterator item = _coordsHor.begin(); item != _coordsHor.end(); ++item)
				{
					if ((*item) == (mTextureRegion.height - offset))
						return;
				}
				_coordsHor.push_back(mTextureRegion.height - offset);
			}
			else if (_corner.isRight())
			{
				for (std::vector<int>::iterator item = _coordsVert.begin(); item != _coordsVert.end(); ++item)
				{
					if ((*item) == (mTextureRegion.width - offset))
						return;
				}
				_coordsVert.push_back(mTextureRegion.width - offset);
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

		for (size_t index = 0; index < mHorizontalBlackSelectors.size(); ++index)
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

		for (size_t index = 0; index < mVerticalBlackSelectors.size(); ++index)
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
