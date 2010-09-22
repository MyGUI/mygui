/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "SeparatorTextureControl.h"
#include "SettingsManager.h"
#include "CommandManager.h"

namespace tools
{
	SeparatorTextureControl::SeparatorTextureControl(MyGUI::Widget* _parent) :
		TextureToolControl(_parent),
		mTextureVisible(false),
		mHorizontalSelectorControl(nullptr),
		mVerticalSelectorControl(nullptr),
		mGridStep(0),
		mValue(0)
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

		CommandManager::getInstance().registerCommand("Command_MoveLeft", MyGUI::newDelegate(this, &SeparatorTextureControl::CommandMoveLeft));
		CommandManager::getInstance().registerCommand("Command_MoveRight", MyGUI::newDelegate(this, &SeparatorTextureControl::CommandMoveRight));
		CommandManager::getInstance().registerCommand("Command_MoveTop", MyGUI::newDelegate(this, &SeparatorTextureControl::CommandMoveTop));
		CommandManager::getInstance().registerCommand("Command_MoveBottom", MyGUI::newDelegate(this, &SeparatorTextureControl::CommandMoveBottom));
		CommandManager::getInstance().registerCommand("Command_GridMoveLeft", MyGUI::newDelegate(this, &SeparatorTextureControl::CommandGridMoveLeft));
		CommandManager::getInstance().registerCommand("Command_GridMoveRight", MyGUI::newDelegate(this, &SeparatorTextureControl::CommandGridMoveRight));
		CommandManager::getInstance().registerCommand("Command_GridMoveTop", MyGUI::newDelegate(this, &SeparatorTextureControl::CommandGridMoveTop));
		CommandManager::getInstance().registerCommand("Command_GridMoveBottom", MyGUI::newDelegate(this, &SeparatorTextureControl::CommandGridMoveBottom));

		mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &SeparatorTextureControl::notifySettingsChanged);

		initialiseAdvisor();
	}

	SeparatorTextureControl::~SeparatorTextureControl()
	{
		shutdownAdvisor();

		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &SeparatorTextureControl::notifySettingsChanged);

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
			MyGUI::Align corner = getCorner();

			int offset = 0;
			if (MyGUI::utility::parseComplex(value, offset))
			{
				mValue = offset;
				if (corner.isTop())
					mHorizontalSelectorControl->setCoord(MyGUI::IntCoord(0, mValue, mTextureRegion.width, 1));
				else if (corner.isLeft())
					mVerticalSelectorControl->setCoord(MyGUI::IntCoord(mValue, 0, 1, mTextureRegion.height));
				else if (corner.isBottom())
					mHorizontalSelectorControl->setCoord(MyGUI::IntCoord(0, mTextureRegion.height - mValue, mTextureRegion.width, 1));
				else if (corner.isRight())
					mVerticalSelectorControl->setCoord(MyGUI::IntCoord(mTextureRegion.width - mValue, 0, 1, mTextureRegion.height));
			}
		}
	}

	void SeparatorTextureControl::updateVisible()
	{
		mHorizontalSelectorControl->setVisible(false);
		mVerticalSelectorControl->setVisible(false);

		if (getCurrentSeparator() != nullptr)
		{
			MyGUI::Align corner = getCorner();

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
		MyGUI::IntPoint pointValue(mVerticalSelectorControl->getPosition().left, mHorizontalSelectorControl->getPosition().top);

		// снапим к гриду
		if (!MyGUI::InputManager::getInstance().isShiftPressed())
		{
			MyGUI::IntPoint point = pointValue;
			MyGUI::IntCoord actionScaleH = mHorizontalSelectorControl->getActionScale();
			MyGUI::IntCoord actionScaleV = mVerticalSelectorControl->getActionScale();

			if (actionScaleV.left != 0)
			{
				point.left = toGrid(point.left + (mGridStep / 2));
			}

			if (actionScaleH.top != 0)
			{
				point.top = toGrid(point.top + (mGridStep / 2));
			}

			if (point != pointValue)
			{
				pointValue = point;
				MyGUI::Align corner = getCorner();
				if (corner.isTop())
					mHorizontalSelectorControl->setCoord(MyGUI::IntCoord(0, pointValue.top, mTextureRegion.width, 1));
				else if (corner.isLeft())
					mVerticalSelectorControl->setCoord(MyGUI::IntCoord(pointValue.left, 0, 1, mTextureRegion.height));
				else if (corner.isBottom())
					mHorizontalSelectorControl->setCoord(MyGUI::IntCoord(0, pointValue.top, mTextureRegion.width, 1));
				else if (corner.isRight())
					mVerticalSelectorControl->setCoord(MyGUI::IntCoord(pointValue.left, 0, 1, mTextureRegion.height));
			}
		}

		MyGUI::Align corner = getCorner();
		if (corner.isTop())
			mValue = pointValue.top;
		else if (corner.isLeft())
			mValue = pointValue.left;
		else if (corner.isBottom())
			mValue = mTextureRegion.height - pointValue.top;
		else if (corner.isRight())
			mValue = mTextureRegion.width -  pointValue.left;

		if (getCurrentSeparator() != nullptr)
			getCurrentSeparator()->getPropertySet()->setPropertyValue("Offset", MyGUI::utility::toString(mValue), mTypeName);
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

	int SeparatorTextureControl::toGrid(int _value)
	{
		if (mGridStep < 1)
			return _value;
		return _value / mGridStep * mGridStep;
	}

	void SeparatorTextureControl::notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName)
	{
		if (_sectorName == "Settings")
		{
			if (_propertyName == "Grid")
				mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		}
	}

	void SeparatorTextureControl::CommandMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		MyGUI::Align corner = getCorner();
		if (corner.isLeft())
			mValue --;
		else if (corner.isRight())
			mValue ++;

		updateFromPointValue();

		_result = true;
	}

	void SeparatorTextureControl::CommandMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		MyGUI::Align corner = getCorner();
		if (corner.isLeft())
			mValue ++;
		else if (corner.isRight())
			mValue --;

		updateFromPointValue();

		_result = true;
	}

	void SeparatorTextureControl::CommandMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		MyGUI::Align corner = getCorner();
		if (corner.isTop())
			mValue --;
		else if (corner.isBottom())
			mValue ++;

		updateFromPointValue();

		_result = true;
	}

	void SeparatorTextureControl::CommandMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		MyGUI::Align corner = getCorner();
		if (corner.isTop())
			mValue ++;
		else if (corner.isBottom())
			mValue --;

		updateFromPointValue();

		_result = true;
	}

	void SeparatorTextureControl::CommandGridMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		MyGUI::Align corner = getCorner();
		if (corner.isLeft())
			mValue = toGrid(--mValue);
		else if (corner.isRight())
		{
			int value = mTextureRegion.width - mValue;
			value = toGrid(--value);
			mValue = mTextureRegion.width - value;
		}

		updateFromPointValue();

		_result = true;
	}

	void SeparatorTextureControl::CommandGridMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		MyGUI::Align corner = getCorner();
		if (corner.isLeft())
			mValue = toGrid(mValue + mGridStep);
		else if (corner.isRight())
		{
			int value = mTextureRegion.width - mValue;
			value = toGrid(value + mGridStep);
			mValue = mTextureRegion.width - value;
		}

		updateFromPointValue();

		_result = true;
	}

	void SeparatorTextureControl::CommandGridMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		MyGUI::Align corner = getCorner();
		if (corner.isTop())
			mValue = toGrid(--mValue);
		else if (corner.isBottom())
		{
			int value = mTextureRegion.height - mValue;
			value = toGrid(--value);
			mValue = mTextureRegion.height - value;
		}

		updateFromPointValue();

		_result = true;
	}

	void SeparatorTextureControl::CommandGridMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		MyGUI::Align corner = getCorner();
		if (corner.isTop())
			mValue = toGrid(mValue + mGridStep);
		else if (corner.isBottom())
		{
			int value = mTextureRegion.height - mValue;
			value = toGrid(value + mGridStep);
			mValue = mTextureRegion.height - value;
		}

		updateFromPointValue();

		_result = true;
	}

	bool SeparatorTextureControl::checkCommand()
	{
		return 
			mMainWidget->getRootKeyFocus() &&
			!mHorizontalSelectorControl->getCapture() &&
			!mVerticalSelectorControl->getCapture();
	}

	void SeparatorTextureControl::onMouseButtonClick(const MyGUI::IntPoint& _point)
	{
		MyGUI::Align corner = getCorner();
		if (corner.isTop())
			mValue = _point.top;
		else if (corner.isLeft())
			mValue = _point.left;
		else if (corner.isBottom())
			mValue = mTextureRegion.height - _point.top;
		else if (corner.isRight())
			mValue = mTextureRegion.width -  _point.left;

		updateFromPointValue();
	}

	void SeparatorTextureControl::updateFromPointValue()
	{
		MyGUI::Align corner = getCorner();
		if (corner.isTop())
			mHorizontalSelectorControl->setCoord(MyGUI::IntCoord(0, mValue, mTextureRegion.width, 1));
		else if (corner.isLeft())
			mVerticalSelectorControl->setCoord(MyGUI::IntCoord(mValue, 0, 1, mTextureRegion.height));
		else if (corner.isBottom())
			mHorizontalSelectorControl->setCoord(MyGUI::IntCoord(0, mTextureRegion.height - mValue, mTextureRegion.width, 1));
		else if (corner.isRight())
			mVerticalSelectorControl->setCoord(MyGUI::IntCoord(mTextureRegion.width - mValue, 0, 1, mTextureRegion.height));

		if (getCurrentSeparator() != nullptr)
			getCurrentSeparator()->getPropertySet()->setPropertyValue("Offset", MyGUI::utility::toString(mValue), mTypeName);
	}

	MyGUI::Align SeparatorTextureControl::getCorner()
	{
		if (getCurrentSeparator() != nullptr)
			return getCurrentSeparator()->getCorner();
		return MyGUI::Align::Default;
	}

} // namespace tools
