/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "StateTextureControl.h"
#include "SkinManager.h"
#include "CommandManager.h"
#include "Localise.h"
#include "Grid.h"

namespace tools
{
	StateTextureControl::StateTextureControl(MyGUI::Widget* _parent) :
		TextureToolControl(_parent),
		mAreaSelectorControl(nullptr)
	{
		mTypeName = MyGUI::utility::toString((size_t)this);

		//immediately draw a frames for states
		std::vector<MyGUI::IntCoord> coords(8);
		drawUnselectedStates(coords);

		addSelectorControl(mAreaSelectorControl);

		mAreaSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &StateTextureControl::notifyChangePosition);

		CommandManager::getInstance().registerCommand("Command_MoveLeft", MyGUI::newDelegate(this, &StateTextureControl::CommandMoveLeft));
		CommandManager::getInstance().registerCommand("Command_MoveRight", MyGUI::newDelegate(this, &StateTextureControl::CommandMoveRight));
		CommandManager::getInstance().registerCommand("Command_MoveTop", MyGUI::newDelegate(this, &StateTextureControl::CommandMoveTop));
		CommandManager::getInstance().registerCommand("Command_MoveBottom", MyGUI::newDelegate(this, &StateTextureControl::CommandMoveBottom));
		CommandManager::getInstance().registerCommand("Command_GridMoveLeft", MyGUI::newDelegate(this, &StateTextureControl::CommandGridMoveLeft));
		CommandManager::getInstance().registerCommand("Command_GridMoveRight", MyGUI::newDelegate(this, &StateTextureControl::CommandGridMoveRight));
		CommandManager::getInstance().registerCommand("Command_GridMoveTop", MyGUI::newDelegate(this, &StateTextureControl::CommandGridMoveTop));
		CommandManager::getInstance().registerCommand("Command_GridMoveBottom", MyGUI::newDelegate(this, &StateTextureControl::CommandGridMoveBottom));

		initialiseAdvisor();

		updateCaption();
	}

	StateTextureControl::~StateTextureControl()
	{
		shutdownAdvisor();

		mAreaSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &StateTextureControl::notifyChangePosition);
	}

	void StateTextureControl::updateVisible()
	{
		MyGUI::UString visible;

		if (getCurrentState() != nullptr)
			visible = getCurrentState()->getPropertySet()->getPropertyValue("Visible");

		mAreaSelectorControl->setVisible(visible == "True");
	}

	void StateTextureControl::updatePosition()
	{
		MyGUI::UString value;

		if (getCurrentState() != nullptr)
			value = getCurrentState()->getPropertySet()->getPropertyValue("Position");

		MyGUI::IntPoint position;
		if (MyGUI::utility::parseComplex(value, position.left, position.top))
		{
			mPointValue = position;
			mAreaSelectorControl->setPosition(mPointValue);
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
			if (mSizeValue != coord.size())
			{
				mSizeValue = coord.size();
				updateSelectorsSize();
			}
		}
	}

	void StateTextureControl::updateSelectorsSize()
	{
		mAreaSelectorControl->setSize(mSizeValue);

		updateUnselectedStates();
	}

	void StateTextureControl::notifyChangePosition()
	{
		mPointValue = mAreaSelectorControl->getPosition();

		// снапим к гриду
		if (!MyGUI::InputManager::getInstance().isShiftPressed())
		{
			MyGUI::IntPoint point = mPointValue;
			MyGUI::IntCoord actionScale = mAreaSelectorControl->getActionScale();

			if (actionScale.left != 0)
			{
				point.left = Grid::getInstance().toGrid(point.left);
			}

			if (actionScale.top != 0)
			{
				point.top = Grid::getInstance().toGrid(point.top);
			}

			if (point != mPointValue)
			{
				mPointValue = point;
				mAreaSelectorControl->setPosition(mPointValue);
			}
		}

		if (getCurrentState() != nullptr)
			getCurrentState()->getPropertySet()->setPropertyValue("Position", mPointValue.print(), mTypeName);
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

				for (std::vector<MyGUI::IntCoord>::iterator item = _coords.begin(); item != _coords.end(); ++item)
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

		for (size_t index = 0; index < mBlackSelectors.size(); ++index)
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

	void StateTextureControl::onMouseButtonClick(const MyGUI::IntPoint& _point)
	{
		mPointValue.left = _point.left - (mSizeValue.width / 2);
		mPointValue.top = _point.top - (mSizeValue.height / 2);

		updateFromPointValue();
	}

	void StateTextureControl::updateFromPointValue()
	{
		mAreaSelectorControl->setPosition(mPointValue);

		if (getCurrentState() != nullptr)
			getCurrentState()->getPropertySet()->setPropertyValue("Position", mPointValue.print(), mTypeName);
	}

	void StateTextureControl::CommandMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mPointValue.left --;
		updateFromPointValue();

		_result = true;
	}

	void StateTextureControl::CommandMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mPointValue.left ++;
		updateFromPointValue();

		_result = true;
	}

	void StateTextureControl::CommandMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mPointValue.top --;
		updateFromPointValue();

		_result = true;
	}

	void StateTextureControl::CommandMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mPointValue.top ++;
		updateFromPointValue();

		_result = true;
	}

	void StateTextureControl::CommandGridMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mPointValue.left = Grid::getInstance().toGrid(mPointValue.left, Grid::Previous);
		updateFromPointValue();

		_result = true;
	}

	void StateTextureControl::CommandGridMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mPointValue.left = Grid::getInstance().toGrid(mPointValue.left, Grid::Next);
		updateFromPointValue();

		_result = true;
	}

	void StateTextureControl::CommandGridMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mPointValue.top = Grid::getInstance().toGrid(mPointValue.top, Grid::Previous);
		updateFromPointValue();

		_result = true;
	}

	void StateTextureControl::CommandGridMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mPointValue.top = Grid::getInstance().toGrid(mPointValue.top, Grid::Next);
		updateFromPointValue();

		_result = true;
	}

	void StateTextureControl::onChangeScale()
	{
		updateCaption();
	}

	void StateTextureControl::updateCaption()
	{
		if (getActivate())
		{
			int scale = (int)(getScale() * (double)100);
			addUserTag("CurrentScale", MyGUI::utility::toString(scale));

			CommandManager::getInstance().executeCommand("Command_UpdateAppCaption");
		}
	}

	void StateTextureControl::onChangeActivate()
	{
		if (getActivate())
			updateCaption();
	}

} // namespace tools
